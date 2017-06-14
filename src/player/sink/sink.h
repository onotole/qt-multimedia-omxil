#ifndef __SINK_H__
#define __SINK_H__

#include "../omx/component.h"
#include "../omx/util.h"
#include "../src/src.h"

enum class PlaybackState { STOPPED, PAUSED, PLAYING };

class ESSink {
public:
    ESSink(const Source::stream_config_t & config);

    omx::Buffer & get_buffer();
    void feed(omx::Buffer & buffer);

    void play();
    void pause();
    PlaybackState get_playback_state() const { return _state; }

private:
    struct OMXInit {
        OMXInit() {
            auto error = OMX_Init();
            if (error != OMX_ErrorNone) {
                ERROR << "Failed to init OMX: " << omx::error_to_string(error);
                throw std::runtime_error("failed to initialize OMX client");
            }
        }
        ~OMXInit() { OMX_Deinit(); }
    } _omx_init;

    omx::Component _decoder;
    omx::Component _clock;
    omx::Component _scheduler;
    omx::Component _renderer;

    bool _start_set = false;
    PlaybackState _state = PlaybackState::STOPPED;
};

#endif // __SINK_H__
