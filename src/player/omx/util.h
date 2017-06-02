#ifndef __MEDIA_OMX_UTIL_H__
#define __MEDIA_OMX_UTIL_H__

#include <iostream>
#define OMX_SKIP64BIT
#include <OMX_Core.h>

struct LogRecord {
    LogRecord(std::ostream & stream) : _stream(stream) {}
    ~LogRecord() { _stream << std::endl; }

    template<typename T>
    LogRecord & operator << (const T & t) {
        _stream << t;
        return *this;
    }

    static LogRecord trace() { return LogRecord(std::cout); }
    static LogRecord debug() { return LogRecord(std::cout); }
    static LogRecord info()  { return LogRecord(std::cout); }
    static LogRecord error() { return LogRecord(std::cerr); }

private:
    std::ostream & _stream;
};

struct DummyRecord {
    template <typename T>
    DummyRecord & operator << (const T &) {
        return *this;
    }
    static DummyRecord dummy() { return DummyRecord(); }
};

#define TRACE DummyRecord::dummy() << "TRACE: "
#define DEBUG LogRecord::debug() << "DEBUG: "
#define INFO  LogRecord::info () << "INFO: "
#define ERROR LogRecord::error() << "ERROR: "

namespace omx {

const char * event_to_string(OMX_EVENTTYPE event);
const char * command_to_string(OMX_COMMANDTYPE command);
const char * state_to_string(OMX_STATETYPE state);
const char * error_to_string(OMX_ERRORTYPE error);

} // namespace omx

#endif // __MEDIA_OMX_UTIL_H__
