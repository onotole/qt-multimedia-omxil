#include <iostream>
#include <unistd.h>
#include "util.h"
#include "component.h"
#include "core.h"

namespace omx {

Component::Component(const std::string & name)
	: _name(name)
	, _callbacks {
		[](OMX_HANDLETYPE, OMX_PTR self, OMX_EVENTTYPE event, OMX_U32 d1, OMX_U32 d2, OMX_PTR data) {
			return static_cast<Component *>(self)->on_event(event, d1, d2, data);
		},
		[](OMX_HANDLETYPE, OMX_PTR self, OMX_BUFFERHEADERTYPE * buffer) {
			return static_cast<Component *>(self)->on_empty_buffer(buffer);
		},
		[](OMX_HANDLETYPE, OMX_PTR self, OMX_BUFFERHEADERTYPE * buffer) {
			return static_cast<Component *>(self)->on_fill_buffer(buffer);
		}
	}
{
	TRACE << "Creating component: " << name << " ...";
	auto error = OMX_GetHandle(&_handle, (OMX_STRING)_name.c_str(), this, &_callbacks);
	if (error != OMX_ErrorNone) {
		ERROR << "[" << _name << "]: Failed to get component handle: " << error_to_string(error);
		throw std::runtime_error("failed to create component");
	}

	static const OMX_INDEXTYPE domains[] {
		OMX_IndexParamAudioInit,
		OMX_IndexParamVideoInit,
		OMX_IndexParamImageInit,
		OMX_IndexParamOtherInit
	};

	std::vector<Port*> outputs;
	for (const auto & domain : domains) {
		OMX_PortParam ports;
		error = get_parameter(domain, &ports);
		if (error != OMX_ErrorNone) {
			throw std::runtime_error("failed to create component");
		}
		for (size_t p = 0; p < ports.nPorts; ++p) {
			OMX_ParamPortDefinition port_def;
			port_def.nPortIndex = ports.nStartPortNumber + p;
			error = get_parameter(OMX_IndexParamPortDefinition, &port_def);
			if (error != OMX_ErrorNone) {
				throw std::runtime_error("failed to create component");
			}
			auto port = new Port(this, port_def.eDomain, port_def.eDir, port_def.nPortIndex);
			if (port->direction() == OMX_DirInput)
				_ports.push_back(port);
			else if (port->direction() == OMX_DirOutput)
				outputs.push_back(port);
			else
				throw std::runtime_error("failed to create component");
		}
	}

	size_t inputs = _ports.size();
	_ports.insert(_ports.end(), outputs.begin(), outputs.end());
	_out_start = _ports.begin() + inputs;

	error = set_state(OMX_StateIdle);
	if (error != OMX_ErrorNone) {
		throw std::runtime_error("failed to create component");
	}
	DEBUG << "component " << _name << " created: ports = "
		  << inputs << "(I), " << outputs.size() << "(O)";
}

Component::~Component() {
	for (auto port : _ports)
		delete port;
}

const std::string & Component::name() const {
	return _name;
}

OMX_HANDLETYPE Component::handle() const {
	return _handle;
}

Port * Component::port(OMX_U32 index) {
	return index < _ports.size() ? _ports[index] : nullptr;
}

Port * Component::input(OMX_U32 index) {
	return index < _out_start - _ports.begin() ? _ports[index] : nullptr;
}

Port * Component::output(OMX_U32 index) {
	return index < _ports.end() - _out_start ? *(_out_start + index) : nullptr;
}

OMX_ERRORTYPE Component::set_parameter(OMX_INDEXTYPE param, OMX_PTR value) {
	auto error = OMX_SetParameter(_handle, param, value);
	if (error != OMX_ErrorNone) {
		ERROR << "[" << _name << "]: Failed to set parameter: " << error_to_string(error);
	}
	return error;
}

OMX_ERRORTYPE Component::get_parameter(OMX_INDEXTYPE param, OMX_PTR value) const {
	auto error = OMX_GetParameter(_handle, param, value);
	if (error != OMX_ErrorNone) {
		ERROR << "[" << _name << "]: Failed to get parameter: " << error_to_string(error);
	}
	return error;
}

OMX_ERRORTYPE Component::set_config(OMX_INDEXTYPE config, OMX_PTR value) {
	auto error = OMX_SetConfig(_handle, config, value);
	if (error != OMX_ErrorNone) {
		ERROR << "[" << _name << "]: Failed to set config: " << error_to_string(error);
	}
	return error;
}

OMX_ERRORTYPE Component::get_config(OMX_INDEXTYPE config, OMX_PTR value) const {
	auto error = OMX_GetConfig(_handle, config, value);
	if (error != OMX_ErrorNone) {
		ERROR << "[" << _name << "]: Failed to get config: " << error_to_string(error);
	}
	return error;
}

OMX_ERRORTYPE Component::set_state(OMX_STATETYPE state) {
	if (get_state() == state)
		return OMX_ErrorNone;
	auto error = send_command(OMX_CommandStateSet, state, nullptr);
	if (error != OMX_ErrorNone) {
		ERROR << "[" << _name << "]: Failed to set state: " << error_to_string(error);
	}
	return error;
}

OMX_STATETYPE Component::get_state() const {
	OMX_STATETYPE state = OMX_StateInvalid;
	auto error = OMX_GetState(_handle, &state);
	if (error != OMX_ErrorNone) {
		ERROR << "[" << _name << "]: Failed to get state: " << error_to_string(error);
	}
	return state;
}

OMX_ERRORTYPE Component::send_command(OMX_COMMANDTYPE command, OMX_U32 param, OMX_PTR data) {
	auto error = OMX_SendCommand(_handle, command, param, data);
	if (error != OMX_ErrorNone) {
		ERROR << "[" << _name << "]: Failed to send command: " << error_to_string(error);
	}
	return error;
}

void Component::wait(OMX_STATETYPE state) {
	std::unique_lock<std::mutex> lock(_wait_mutex);
	while (get_state() != state) {
		_wait_cv.wait(lock);
	}
}

void Component::wait(OMX_EVENTTYPE event) {
	std::unique_lock<std::mutex> lock(_wait_mutex);
	_wait_event = {event, false};
	while (!_wait_event.occured) {
		_wait_cv.wait(lock);
	}
	_wait_event = {OMX_EventMax, false};
}

void Component::set_event_handler(OMX_EVENTTYPE event, event_callback_t && callback) {
	_event_callbacks.emplace(std::make_pair(event, callback));
}

void Component::set_empty_buffer_handler(buffer_callback_t && callback) {
	_empty_buffer_callback = callback;
}

void Component::set_fill_buffer_handler(buffer_callback_t && callback) {
	_fill_buffer_callback = callback;
}

OMX_ERRORTYPE Component::on_event(OMX_EVENTTYPE event, OMX_U32 d1, OMX_U32 d2, OMX_PTR data) {
	std::string misc_info;
	switch (event) {
		case OMX_EventError:
			ERROR << "[" << _name << "]: Processing error: "
				  << error_to_string(OMX_ERRORTYPE(d1));
			break;
		case OMX_EventCmdComplete:
			misc_info = d1 == OMX_CommandStateSet ?
						state_to_string(OMX_STATETYPE(d2)) : std::to_string(d2);
			DEBUG << "[" << _name << "]: " << command_to_string(OMX_COMMANDTYPE(d1))
				  << ": " << misc_info;
			break;
		case OMX_EventPortSettingsChanged:
			DEBUG << "[" << _name << "]: " << event_to_string(event) << ": " << d1;
			break;
		default:
			DEBUG << "[" << _name << "]: Event: " << event_to_string(event)
				  << ", params: " << d1 << ", " << d2 << ", " << data;
			break;
	}

	if (event == OMX_EventError) {
		ERROR << "[" << _name << "]: Processing error: " << error_to_string(OMX_ERRORTYPE(d1));
	} else {

	}
	// Notify waiter on state chage
	if (event == OMX_EventCmdComplete && d1 == OMX_CommandStateSet) {
		_wait_cv.notify_one();
	}
	auto it = _event_callbacks.find(event);
	if (it != _event_callbacks.end() && it->second) {
		return it->second(d1, d2, data);
	}
	{
		std::unique_lock<std::mutex> lock(_wait_mutex);
		if (event == _wait_event.event) {
			_wait_event.occured = true;
			_wait_cv.notify_one();
		}
	}
	return OMX_ErrorNone;
}

OMX_ERRORTYPE Component::on_empty_buffer(OMX_BUFFERHEADERTYPE * buffer) {
	static size_t counter = 0;
	TRACE << "On Empty: #" << counter++ << ", comp = " << _name;
	input()->unlock_buffer(buffer);
	if(_empty_buffer_callback)
		return _empty_buffer_callback(buffer);
	return OMX_ErrorNone;
}

OMX_ERRORTYPE Component::on_fill_buffer(OMX_BUFFERHEADERTYPE * buffer) {
	static size_t counter = 0;
	TRACE << "On Fill: #" << counter++ << ", comp = " << _name;
	// TODO: this should be called after fill_buffer callback
	output()->unlock_buffer(buffer);
	if(_fill_buffer_callback)
		return _fill_buffer_callback(buffer);
	return OMX_ErrorNone;
}

} // namespace omx
