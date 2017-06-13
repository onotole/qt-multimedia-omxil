#ifndef __MEDIA_OMX_UTIL_H__
#define __MEDIA_OMX_UTIL_H__

#include <iostream>
#define OMX_SKIP64BIT
#include <OMX_Core.h>

#ifdef OMX_SKIP64BIT
static inline OMX_TICKS ToOMXTime(int64_t pts)
{
  OMX_TICKS ticks;
  ticks.nLowPart = pts;
  ticks.nHighPart = pts >> 32;
  return ticks;
}
static inline int64_t FromOMXTime(OMX_TICKS ticks)
{
  int64_t pts = ticks.nLowPart | ((uint64_t)(ticks.nHighPart) << 32);
  return pts;
}
#else
#define FromOMXTime(x) (x)
#define ToOMXTime(x) (x)
#endif

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
