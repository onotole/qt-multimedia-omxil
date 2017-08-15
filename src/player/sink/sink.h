#ifndef __SINK_H__
#define __SINK_H__

#include "../omx/component.h"
#include "../omx/util.h"
#include "../src/src.h"

enum class PlaybackState { STOPPED, PAUSED, PLAYING };

class ESSink {
public:
    struct rect_t { int x, y, w, h; };

    ESSink(const Source::stream_config_t & config);

    omx::Buffer & get_buffer();
    void feed(omx::Buffer & buffer);

    void play();
    void pause();
    PlaybackState get_playback_state() const { return _state; }

    void set_output_rect(const rect_t & rect);

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

    void set_display_region();
    void update_layer();
    void update_output_rect();

    omx::Component _decoder;
    omx::Component _clock;
    omx::Component _scheduler;
    omx::Component _renderer;

    static int _next_id;
    int _id;

    bool _start_set = false;
    PlaybackState _state = PlaybackState::STOPPED;

    rect_t _input_rect  = {0, 0, 0, 0};
    rect_t _output_rect = {0, 0, 0, 0};
};

#endif // __SINK_H__
