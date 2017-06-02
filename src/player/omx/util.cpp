#include "util.h"

namespace omx {

const char * event_to_string(OMX_EVENTTYPE event) {
	static const char * _events[] = {
		"Command Complete",
		"Error",
		"Mark",
		"Port Settings Changed",
		"Buffer Flag",
		"Resources Acquired",
		"Component Resumed",
		"Dynamic Resources Available",
		"Port Format Detected"
	};
	if (event < sizeof(_events)/sizeof(_events[0]))
		return _events[event];
	return "Unknown Event";
}

const char * command_to_string(OMX_COMMANDTYPE command) {
	static const char * _commands[] = {
		"State Set",
		"Flush",
		"Port Disable",
		"Port Enable",
		"Mark Buffer"
	};
	if (command < sizeof(_commands)/sizeof(_commands[0]))
		return _commands[command];
	return "Unknown Command";
}

const char * state_to_string(OMX_STATETYPE state) {
	static const char * _states[] = {
		"Invalid",
		"Loaded",
		"Idle",
		"Executing",
		"Pause",
		"Wait For Resources"
	};
	if (state < sizeof(_states)/sizeof(_states[0]))
		return _states[state];
	return "Unknown State";
}

const char * error_to_string(OMX_ERRORTYPE error) {
	static const char * _errors[] = {
		"Insufficient Resources",
		"Undefined",
		"Invalid Component Name",
		"Component Not Found",
		"Invalid Component",
		"Bad Parameter",
		"Not Implemented",
		"Underflow",
		"Overflow",
		"Hardware",
		"Invalid State",
		"Stream Corrupt",
		"Ports Not Compatible",
		"Resources Lost",
		"No More",
		"Version Mismatch",
		"Not Ready",
		"Timeout",
		"Same State",
		"Resources Preempted",
		"Port Unresponsive During Allocation",
		"Port Unresponsive During Deallocation",
		"Port Unresponsive During Stop",
		"Incorrect State Transition",
		"Incorrect State Operation",
		"Unsupported Setting",
		"Unsupported Index",
		"Bad Port Index",
		"Port Unpopulated",
		"Component Suspended",
		"Dynamic Resources Unavailable",
		"Mb Errors In Frame",
		"Format Not Detected",
		"Content Pipe Open Failed",
		"Seperate Tables Used",
		"Tunneling Unsupported"
	};
	if (error == OMX_ErrorNone)
		return "None";
	OMX_U32 index = OMX_U32(error) - 0x80001000U;
	if (index < sizeof(_errors)/sizeof(_errors[0]))
		return _errors[index];
	return "Unknown Error";
}

}
