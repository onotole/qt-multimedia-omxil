#ifndef __MEDIA_OMX_BUFFER_H__
#define __MEDIA_OMX_BUFFER_H__

#include <memory>
#include <mutex>
#include <condition_variable>
#include <vector>
#include "core.h"

namespace omx {

class Buffer {
public:
	Buffer(OMX_HANDLETYPE handle, OMX_U32 index, OMX_U8 * data, OMX_U32 size);
	// egl image buffer
	Buffer(OMX_HANDLETYPE handle, OMX_U32 index);
	~Buffer();
	OMX_BUFFERHEADERTYPE * header();

	OMX_ERRORTYPE write(OMX_U8 * data, OMX_U32 size);
	const OMX_U8 * data() const;
	OMX_U32 size() const;

	OMX_ERRORTYPE use_egl_image(void * image, void * token);

	bool lock();
	void unlock();

private:
	OMX_BUFFERHEADERTYPE * _header = nullptr;
	OMX_HANDLETYPE _handle = nullptr;
	OMX_U32 _index = 0;
	bool _locked = false;
	bool _egl_image = false;
};

class Buffers {
public:
	Buffers(OMX_HANDLETYPE handle, OMX_U32 index, OMX_U32 count, OMX_U32 size);
	// egl image buffer
	Buffers(OMX_HANDLETYPE handle, OMX_U32 index, OMX_U32 count);
	~Buffers();

	Buffer & get();
	void unlock(OMX_BUFFERHEADERTYPE * buffer);
	OMX_ERRORTYPE use_egl_image(void * image);

private:
	OMX_U32 next_unlocked_buffer();

	std::mutex _mutex;
	std::condition_variable _cv;
	std::vector<Buffer> _buffers;
	OMX_U32 _next_index = 0;
	OMX_U8 * _data;
	bool _egl_image = false;
	OMX_U32 _current_egl_buffer = 0;
};

} // namespace omx

#endif // __MEDIA_OMX_BUFFER_H__
