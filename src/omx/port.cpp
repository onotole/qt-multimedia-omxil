#include "util.h"
#include "port.h"
#include "component.h"

namespace omx {

Port::Port(Component * component, OMX_PORTDOMAINTYPE domain, OMX_DIRTYPE direction, OMX_U32 index)
	: _component(component), _domain(domain), _direction(direction), _index(index) {
	auto error = disable();
	if (error != OMX_ErrorNone) {
		ERROR << "[" << _component->name() << "]: Failed to disable port: "
			  << error_to_string(error);
		throw std::runtime_error("failed to init port");
	}
}

Component * Port::component() {
	return _component;
}

OMX_PORTDOMAINTYPE Port::domain() const {
	return _domain;
}

OMX_DIRTYPE Port::direction() const {
	return _direction;
}

OMX_U32 Port::index() const {
	return _index;
}

OMX_ERRORTYPE Port::enable() {
	return _component->send_command(OMX_CommandPortEnable, _index, nullptr);
}

OMX_ERRORTYPE Port::disable() {
	return _component->send_command(OMX_CommandPortDisable, _index, nullptr);
}

OMX_ERRORTYPE Port::flush() {
	return _component->send_command(OMX_CommandFlush, _index, nullptr);
}

OMX_ERRORTYPE Port::connect(Port * other) {
	if (_peer) disconnect();
	auto error = OMX_SetupTunnel(_component->handle(), _index, other->_component->handle(), other->_index);
	if (error != OMX_ErrorNone) {
		ERROR << "[" << _component->name() << "]: Failed to setup tunnel: "
			  << error_to_string(error);
		return error;
	}
	error = enable();
	if (error != OMX_ErrorNone) {
		ERROR << "[" << _component->name() << "]: Failed to enable port: "
			  << error_to_string(error);
		return error;
	}
	error = other->enable();
	if (error != OMX_ErrorNone) {
		ERROR << "[" << _component->name() << "]: Failed to enable port: "
			  << error_to_string(error);
	}
	return error;
}

OMX_ERRORTYPE Port::disconnect() {
	_peer = nullptr;
}

// buffers
OMX_ERRORTYPE Port::alloc_buffers(OMX_U32 buffers, bool use_egl_images) {
	if (_buffers)
		return OMX_ErrorNone;
	OMX_ParamPortDefinition pd;
	pd.nPortIndex = _index;
	auto error = _component->get_parameter(OMX_IndexParamPortDefinition, &pd);
	if(error != OMX_ErrorNone) {
		ERROR << "[" << _component->name() << "]: Failed to allocate buffers: "
			  << error_to_string(error);
		return error;
	}
	OMX_U32 actual_buffers = pd.nBufferCountActual;
	if (buffers == DEFAULT)
		buffers = std::max(actual_buffers, pd.nBufferCountMin);
	if (actual_buffers != buffers) {
		pd.nBufferCountActual = buffers;
		error = _component->set_parameter(OMX_IndexParamPortDefinition, &pd);
		if(error != OMX_ErrorNone) {
			ERROR << "[" << _component->name() << "]: Failed to allocate buffers: "
				  << error_to_string(error);
			return error;
		}
	}
	if (use_egl_images) {
		_buffers.reset(new Buffers(_component->handle(), _index, buffers));
		TRACE << "Alloc " << buffers << " egl images...";
	} else {
		_buffers.reset(new Buffers(_component->handle(), _index, buffers, pd.nBufferSize));
		TRACE << "Alloc " << buffers << " cpu buffers of size " << pd.nBufferSize << " ...";
	}

	return OMX_ErrorNone;
}

Buffer & Port::get_buffer() {
	alloc_buffers();
	return _buffers->get();
}

OMX_ERRORTYPE Port::process_buffer(Buffer & buffer) {
	auto error = OMX_ErrorNone;
	switch (_direction) {
		case OMX_DirInput:
			error = OMX_EmptyThisBuffer(_component->handle(), buffer.header());
			break;
		case OMX_DirOutput:
			error = OMX_FillThisBuffer(_component->handle(), buffer.header());
			break;
	}
	if (error != OMX_ErrorNone) {
		ERROR << "[" << _component->name() << "]: Failed to process buffer: "
			  << error_to_string(error);
	}
	return error;
}

void Port::unlock_buffer(OMX_BUFFERHEADERTYPE * buffer) {
	if(_buffers) _buffers->unlock(buffer);
}

OMX_ERRORTYPE Port::use_egl_image(void * image) {
	auto error = _buffers->use_egl_image(image);
	if (error != OMX_ErrorNone) {
		ERROR << "[" << _component->name() << "]: Failed to set EGLImage: "
			  << error_to_string(error);
	}
	return error;
}

} // namespace omx
