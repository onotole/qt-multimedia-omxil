#ifndef __MEDIA_OMX_UTIL_H__
#define __MEDIA_OMX_UTIL_H__

#include <iostream>
#include <OMX_Core.h>

#define TRACE (std::cout << "TRACE: ")
#define DEBUG (std::cout << "DEBUG: ")
#define INFO  (std::cout << "INFO: ")
#define ERROR (std::cerr << "ERROR: ")

namespace omx {

const char * event_to_string(OMX_EVENTTYPE event);
const char * command_to_string(OMX_COMMANDTYPE command);
const char * state_to_string(OMX_STATETYPE state);
const char * error_to_string(OMX_ERRORTYPE error);

} // namespace omx

#endif // __MEDIA_OMX_UTIL_H__
