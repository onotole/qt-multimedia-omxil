#include "player.h"
#include <unistd.h>
#include "omx/util.h"

Player::Player() {}

Player::~Player() {
    unload();
}

void Player::load(const std::string & uri) {
    DEBUG << "player::load: " << uri;
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
    DEBUG << "payer::unload.";
    {
        std::unique_lock<std::mutex> _(_lock);
        _src.reset();
        _sink.reset();
    }
    if (_data_thread.joinable())
        _data_thread.join();
}

void Player::play() {
    DEBUG << "player::play.";
    if (_sink) _sink->play();
}

void Player::pause() {
    DEBUG << "player::pause.";
    if (_sink) _sink->pause();
}
