#include "player.h"
#include <unistd.h>
#include "omx/util.h"

Player::Player() {}

Player::~Player() {
    unload();
}

void Player::load(const std::string & uri) {
    unload();
    _src.reset(Source::create(uri));
    _sink.reset(new ESSink(_src->stream_config()));
    _data_thread = std::thread([this](){
        while (true) {
            std::unique_lock<std::mutex> _(_lock);
            if (!_src || !_sink) break;
            auto & buffer = _sink->get_buffer();
            PullSource * source = dynamic_cast<PullSource *>(_src.get());
            if (!source->read(buffer)) break;
            _sink->feed(buffer);
        }
    });
}

void Player::unload() {
    {
        std::unique_lock<std::mutex> _(_lock);
        _src.reset();
        _sink.reset();
    }
    if (_data_thread.joinable())
        _data_thread.join();
}

void Player::play() {
}

void Player::pause() {
}
