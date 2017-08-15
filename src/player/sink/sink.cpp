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

int ESSink::_next_id = 1;

ESSink::ESSink(const Source::stream_config_t & config)
    : _decoder("OMX.broadcom.video_decode")
    , _clock("OMX.broadcom.clock")
    , _scheduler("OMX.broadcom.video_scheduler")
    , _renderer("OMX.broadcom.video_render")
    , _id(_next_id++) {
    // configure decoder
    omx::OMX_VideoParamPortFormat format;
    format.eCompressionFormat = config.video_encoding;
    format.xFramerate = config.framerate;
    format.nPortIndex = _decoder.input()->index();
    _decoder.set_parameter(OMX_IndexParamVideoPortFormat, &format);
    _decoder.input()->enable();
    _decoder.input()->alloc_buffers();
    _decoder.set_event_handler(OMX_EventPortSettingsChanged, [&](OMX_U32, OMX_U32, OMX_PTR) {
        omx::OMX_ParamPortDefinition port_def;
        port_def.nPortIndex = _decoder.output()->index();
        _decoder.get_parameter(OMX_IndexParamPortDefinition, &port_def);
        DEBUG << "omx decoder port settigs changed: "
              << port_def.format.video.nFrameWidth << "x" << port_def.format.video.nFrameHeight;
        _input_rect = {0, 0, port_def.format.video.nFrameWidth, port_def.format.video.nFrameHeight};
        set_display_region();
        // update_layer();
        // update_output_rect();
        _decoder.output()->connect(_scheduler.input());
        _scheduler.set_state(OMX_StateExecuting);
        _scheduler.output()->connect(_renderer.input());
        _renderer.set_state(OMX_StateExecuting);
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

void ESSink::set_output_rect(const rect_t & rect) {
    DEBUG << "[" << _id << "]: out rc = {" << rect.x << ", " << rect.y << ", " << rect.w << ", " << rect.h << "} ...";
    _output_rect = rect;
}

void ESSink::update_layer() {
#ifdef OMX_RPI
    static int current_layer = 1;
    omx::OMX_ConfigDisplayRegion display_config;
    display_config.nPortIndex = _renderer.input()->index();
    display_config.set = OMX_DISPLAY_SET_LAYER;
    display_config.layer = current_layer++;
    DEBUG << "[" << _id << "]: setting layer = " << display_config.layer << " ...";
    _renderer.set_config(OMX_IndexConfigDisplayRegion, &display_config);
#endif
}

void ESSink::update_output_rect() {
#ifdef OMX_RPI
    rect_t & rc = _output_rect;
    omx::OMX_ConfigDisplayRegion display_config;
    display_config.nPortIndex = _renderer.input()->index();
    display_config.set = OMX_DISPLAY_SET_DEST_RECT;
    display_config.dest_rect = {rc.x, rc.y, rc.w, rc.h};
    DEBUG << "[" << _id << "]: setting out rect = {" << rc.x << ", " << rc.y << ", " << rc.w << ", " << rc.h << "} ...";
    _renderer.set_config(OMX_IndexConfigDisplayRegion, &display_config);
#endif
}


void ESSink::set_display_region() {
#ifdef OMX_RPI
    static int current_layer = 1;

    omx::OMX_ConfigDisplayRegion display_config;
    display_config.nPortIndex = _renderer.input()->index();
    display_config.set = (OMX_DISPLAYSETTYPE)
            (OMX_DISPLAY_SET_LAYER | OMX_DISPLAY_SET_SRC_RECT | OMX_DISPLAY_SET_DEST_RECT | OMX_DISPLAY_SET_FULLSCREEN);

    display_config.layer = current_layer++;

    display_config.fullscreen = OMX_FALSE;

    display_config.src_rect.x_offset = _input_rect.x;
    display_config.src_rect.y_offset = _input_rect.y;
    display_config.src_rect.width    = _input_rect.w;
    display_config.src_rect.height   = _input_rect.h;

    display_config.dest_rect.x_offset = _output_rect.x;
    display_config.dest_rect.y_offset = _output_rect.y;
    display_config.dest_rect.width    = _output_rect.w;
    display_config.dest_rect.height   = _output_rect.h;

    _renderer.set_config(OMX_IndexConfigDisplayRegion, &display_config);
#endif

}

