#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <cassert>
#include "util.h"
#include "buffer.h"

namespace omx {

Buffer::Buffer(OMX_HANDLETYPE handle, OMX_U32 index, OMX_U8 * data, OMX_U32 size)
	: _handle(handle), _index(index) {
	auto error = OMX_UseBuffer(_handle, &_header, index, nullptr, size, data);
	if (error != OMX_ErrorNone) {
		ERROR << "Failed to use buffer: " << error_to_string(error);
		throw std::runtime_error("failed to init buffer");
	}
}

Buffer::Buffer(OMX_HANDLETYPE handle, OMX_U32 index)
	: _handle(handle), _index(index), _egl_image(true) {}

Buffer::~Buffer() {
	if (!_egl_image) {
		auto error = OMX_FreeBuffer(_handle, _header->nInputPortIndex, _header);
		if (error != OMX_ErrorNone) {
			ERROR << "Failed to free buffer: " << error_to_string(error);
		}
	}
}

OMX_BUFFERHEADERTYPE * Buffer::header() {
	return _header;
}

OMX_ERRORTYPE Buffer::write(OMX_U8 * data, OMX_U32 size) {
	TRACE << "writing " << size << " bytes to buffer...";
	if (_egl_image) {
		return OMX_ErrorIncorrectStateOperation;
	}
	if (size > _header->nAllocLen) {
		return OMX_ErrorBadParameter;
	}
	std::memcpy(_header->pBuffer, data, size);
	_header->nFilledLen = size;
	return OMX_ErrorNone;
}

const OMX_U8 * Buffer::data() const {
	return _egl_image ? nullptr : _header->pBuffer;
}

OMX_U32 Buffer::size() const {
	return _egl_image ? 0 : _header->nAllocLen;
}

OMX_ERRORTYPE Buffer::use_egl_image(void * image, void * token) {
	if (!_egl_image) {
		ERROR << "Failed to use egl image: " << error_to_string(OMX_ErrorBadParameter);
		return OMX_ErrorBadParameter;
	}
	auto error = OMX_UseEGLImage(_handle, &_header, _index, token, image);
	if (error != OMX_ErrorNone) {
		ERROR << "Failed to use egl image: " << error_to_string(error);
		return error;
	}
	return OMX_ErrorNone;
}

bool Buffer::lock() {
	if(_locked)
		return false;
	return (_locked = true);
}

void Buffer::unlock() {
	assert(_locked);
	_locked = false;
}

Buffers::Buffers(OMX_HANDLETYPE handle, OMX_U32 index, OMX_U32 count, OMX_U32 size) {
	_data = (OMX_U8*)std::malloc(size * count);
	_buffers.reserve(count);
	for (OMX_U32 b = 0; b < count; ++b) {
		_buffers.emplace_back(handle, index, _data + b * size, size);
	}
}

Buffers::Buffers(OMX_HANDLETYPE handle, OMX_U32 index, OMX_U32 count) : _egl_image(true) {
	_data = (OMX_U8*)std::malloc(sizeof(void *) * count);
	_buffers.reserve(count);
	for (OMX_U32 b = 0; b < count; ++b) {
		_buffers.emplace_back(handle, index);
	}
}

Buffers::~Buffers() {
	std::free(_data);
}

Buffer & Buffers::get() {
	std::unique_lock<std::mutex> lock(_mutex);
	auto next = next_unlocked_buffer();
	while(next == _buffers.size()) {
		_cv.wait(lock);
		next = next_unlocked_buffer();
	}
	_next_index = (next + 1) % _buffers.size();
	TRACE << "giving away buffer # " << next;
	return _buffers[next];
}

void Buffers::unlock(OMX_BUFFERHEADERTYPE * buffer) {
	std::unique_lock<std::mutex> lock(_mutex);
	OMX_U32 index = _egl_image
					? reinterpret_cast<OMX_U32>(buffer->pAppPrivate)
					: (buffer->pBuffer - _data) / buffer->nAllocLen;
	TRACE << "unlocking buffer # " << index;
	_buffers[index].unlock();
	_cv.notify_one();
}

OMX_ERRORTYPE Buffers::use_egl_image(void * image) {
	std::unique_lock<std::mutex> lock(_mutex);
	if (_current_egl_buffer == _buffers.size()) {
		ERROR << "Failed to use egl image: " << error_to_string(OMX_ErrorNoMore);
		return OMX_ErrorNoMore;
	}
	TRACE << "use egl buffer # " << _current_egl_buffer;
	auto error = _buffers[_current_egl_buffer].use_egl_image
						(image, reinterpret_cast<void *>(_current_egl_buffer));
	++_current_egl_buffer;
	return error;
}

OMX_U32 Buffers::next_unlocked_buffer() {
	OMX_U32 next = _next_index;
	do {
		auto & buffer = _buffers[next];
		if (buffer.lock())
				return next;
		next = (next + 1) % _buffers.size();
	} while (next != _next_index);
    TRACE << "buffer underrun detected";
	return _buffers.size();
}

} // namespace omx
