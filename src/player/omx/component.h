#ifndef __MEDIA_OMX_COMPONENT_H__
#define __MEDIA_OMX_COMPONENT_H__

#include <string>
#include <functional>
#include <map>
#include "port.h"
#include "buffer.h"

namespace omx {

class Component {
public:
	typedef std::function<OMX_ERRORTYPE(OMX_U32, OMX_U32, OMX_PTR)> event_callback_t;
	typedef std::function<OMX_ERRORTYPE(OMX_BUFFERHEADERTYPE *)> buffer_callback_t;

	Component(const std::string & name);
	virtual ~Component();

	const std::string & name() const;
	OMX_HANDLETYPE handle() const;

	// ports accessors
	Port * port(OMX_U32 index);
	Port * input(OMX_U32 index = 0);
	Port * output(OMX_U32 index = 0);

	// configuration
	OMX_ERRORTYPE set_parameter(OMX_INDEXTYPE param, OMX_PTR value);
	OMX_ERRORTYPE get_parameter(OMX_INDEXTYPE param, OMX_PTR value) const;
	OMX_ERRORTYPE set_config(OMX_INDEXTYPE config, OMX_PTR value);
	OMX_ERRORTYPE get_config(OMX_INDEXTYPE config, OMX_PTR value) const;

	// state management
	OMX_ERRORTYPE set_state(OMX_STATETYPE state);
	OMX_STATETYPE get_state() const;

	// commands
	OMX_ERRORTYPE send_command(OMX_COMMANDTYPE command, OMX_U32 param, OMX_PTR data);

	// synchronization
	void wait(OMX_STATETYPE state);
	void wait(OMX_EVENTTYPE event);

	// callbacks
	void set_event_handler(OMX_EVENTTYPE event, event_callback_t && callback);
	void set_empty_buffer_handler(buffer_callback_t && callback);
	void set_fill_buffer_handler(buffer_callback_t && callback);

private:
	OMX_ERRORTYPE on_event(OMX_EVENTTYPE event, OMX_U32 d1, OMX_U32 d2, OMX_PTR data);
	OMX_ERRORTYPE on_empty_buffer(OMX_BUFFERHEADERTYPE * buffer);
	OMX_ERRORTYPE on_fill_buffer(OMX_BUFFERHEADERTYPE * buffer);

	OMX_HANDLETYPE _handle = nullptr;
	OMX_CALLBACKTYPE _callbacks = {};

	std::string _name;

	std::vector<Port*> _ports;
	std::vector<Port*>::iterator _out_start;

	std::map<OMX_EVENTTYPE, event_callback_t> _event_callbacks;
	buffer_callback_t _empty_buffer_callback;
	buffer_callback_t _fill_buffer_callback;

	std::mutex _wait_mutex;
	std::condition_variable _wait_cv;
	struct wait_event_t {
		OMX_EVENTTYPE event;
		bool occured;
	} _wait_event = {OMX_EventMax, false};
};

} // namespace omx

#endif // __MEDIA_OMX_COMPONENT_H__
