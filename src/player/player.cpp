#include "player.h"
#include <unistd.h>
#include "omx/util.h"

Player::Player() {}

void Player::load(const std::string & uri) {
    _src.reset(Source::create(uri));
    _sink.reset(new ESSink(_src->stream_config()));
}

void Player::play() {
    PullSource * source = dynamic_cast<PullSource *>(_src.get());
    while(source) {
        auto & buffer = _sink->get_buffer();
        if (!source->read(buffer))
            source = nullptr;
        _sink->feed(buffer);
    }
}

void Player::pause() {
}
