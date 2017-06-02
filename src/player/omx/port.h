#ifndef __MEDIA_OMX_PORT_H__
#define __MEDIA_OMX_PORT_H__

#include <memory>
#include "core.h"
#include "buffer.h"

namespace omx {

class Component;

class Port {
public:
	Port(Component * component, OMX_PORTDOMAINTYPE domain, OMX_DIRTYPE direction, OMX_U32 index);

	// structure
	Component * component();
	OMX_PORTDOMAINTYPE domain() const;
	OMX_DIRTYPE direction() const;
	OMX_U32 index() const;

	// operations
	OMX_ERRORTYPE enable();
	OMX_ERRORTYPE disable();
	OMX_ERRORTYPE flush();

	// tunnels
	OMX_ERRORTYPE connect(Port * other);
	OMX_ERRORTYPE disconnect();

	// buffers
	OMX_ERRORTYPE alloc_buffers(OMX_U32 buffers = DEFAULT, bool use_egl_images = false);
	Buffer & get_buffer();
	OMX_ERRORTYPE process_buffer(Buffer & buffer);
	void unlock_buffer(OMX_BUFFERHEADERTYPE * buffer);
	OMX_ERRORTYPE use_egl_image(void * image);

private:
	const static OMX_U32 DEFAULT = -1;
	Component * _component;
	OMX_PORTDOMAINTYPE _domain;
	OMX_DIRTYPE _direction;
	OMX_U32 _index;

	Port * _peer = nullptr;

	std::unique_ptr<Buffers> _buffers;
};


} // namespace omx

#endif // __MEDIA_OMX_COMPONENT_H__
