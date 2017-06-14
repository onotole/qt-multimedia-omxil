#include "sink.h"
#include "../omx/util.h"
#include <cassert>
#include <unistd.h>

void debug_buffer_feed(omx::Buffer & buffer) {
    TRACE << "feeding " << buffer.header()->nFilledLen
          << " bytes, flags = " << buffer.header()->nFlags
          << ", pts = " << FromOMXTime(buffer.header()->nTimeStamp)
          << ", data: " << std::hex << int(((char*)buffer.data())[0])
          << " " << int(((char*)buffer.data())[1])
          << " " << int(((char*)buffer.data())[2])
          << " " << int(((char*)buffer.data())[3])
          << " " << int(((char*)buffer.data())[4])
          << " " << int(((char*)buffer.data())[5])
          << " ... " << int(((char*)buffer.data())[buffer.header()->nFilledLen - 2])
          << " " << int(((char*)buffer.data())[buffer.header()->nFilledLen - 1]) << std::dec;
}

ESSink::ESSink(const Source::stream_config_t & config) : _decoder("OMX.broadcom.video_decode"), _clock("OMX.broadcom.clock"),
    _scheduler("OMX.broadcom.video_scheduler"), _renderer("OMX.broadcom.video_render") {
    // configure decoder
    omx::OMX_VideoParamPortFormat format;
    format.eCompressionFormat = config.video_encoding;
    format.xFramerate = config.framerate;
    format.nPortIndex = _decoder.input()->index();
    _decoder.set_parameter(OMX_IndexParamVideoPortFormat, &format);
    _decoder.input()->enable();
    _decoder.input()->alloc_buffers();
    _decoder.set_event_handler(OMX_EventPortSettingsChanged, [&](OMX_U32, OMX_U32, OMX_PTR) {
        DEBUG << "omx decoder port settigs changed...";
        _decoder.output()->connect(_scheduler.input());
        _scheduler.set_state(OMX_StateExecuting);
        _scheduler.output()->connect(_renderer.input());
        _renderer.set_state(OMX_StateExecuting);
#ifdef OMX_RPI
        omx::OMX_ConfigDisplayRegion display_config;
        display_config.nPortIndex = _renderer.input()->index();
        display_config.set = OMX_DISPLAY_SET_LAYER;
        display_config.layer = 1;
        _renderer.set_config(OMX_IndexConfigDisplayRegion, &display_config);
#endif
        if (_state != PlaybackState::PLAYING)
            pause();

        return OMX_ErrorNone;
    });

    // configure clock
    omx::OMX_TimeConfigClockState cstate;
    cstate.eState = OMX_TIME_ClockStateWaitingForStartTime;
    cstate.nWaitMask = 1;
    _clock.set_parameter(OMX_IndexConfigTimeClockState, &cstate);
    _clock.output()->connect(_scheduler.input(1));

    _decoder.set_state(OMX_StateExecuting);
    _clock.set_state(OMX_StateExecuting);

    if (config.extra_data.size()) {
        auto & buffer = _decoder.input()->get_buffer();
        memcpy((char*)buffer.data(), &config.extra_data.front(), config.extra_data.size());
        buffer.header()->nFilledLen = config.extra_data.size();
        buffer.header()->nFlags = OMX_BUFFERFLAG_CODECCONFIG | OMX_BUFFERFLAG_ENDOFFRAME;
        debug_buffer_feed(buffer);
        _decoder.input()->process_buffer(buffer);
    }
}

omx::Buffer & ESSink::get_buffer() {
    return _decoder.input()->get_buffer();
}

void ESSink::feed(omx::Buffer & buffer) {
    debug_buffer_feed(buffer);
    _decoder.input()->process_buffer(buffer);
}

void ESSink::play() {
    omx::OMX_TimeConfigScale scale;
    scale.xScale = (1 << 16);
    _clock.set_config(OMX_IndexConfigTimeScale, &scale);
    _state = PlaybackState::PLAYING;
}

void ESSink::pause() {
    omx::OMX_TimeConfigScale scale;
    scale.xScale = 0;
    _clock.set_config(OMX_IndexConfigTimeScale, &scale);
    _state = PlaybackState::PAUSED;
}
