#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <string>
#include <memory>
#include <thread>
#include "sink/sink.h"
#include "src/src.h"

class Player {
public:
    typedef ESSink::rect_t rect_t;

    Player();
    ~Player();

    void load(const std::string & uri);
    void unload();

    void play();
    void pause();
    PlaybackState get_playback_state() const;

    void set_output_rect(const rect_t & rect) {
        if (_sink) _sink->set_output_rect(rect);
    }

private:
    std::thread _data_thread;
    std::mutex _lock;
    std::unique_ptr<Source> _src;
    std::unique_ptr<ESSink> _sink;
};

#endif // __PLAYER_H__
