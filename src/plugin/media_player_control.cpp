#include "media_player_control.h"

int MediaPlayerControl::_next_id = 1;

MediaPlayerControl::MediaPlayerControl(QObject * parent) : QMediaPlayerControl(parent), _id(_next_id++) {}

MediaPlayerControl::~MediaPlayerControl() {}

QMediaPlayer::State MediaPlayerControl::state() const {
    return QMediaPlayer::StoppedState;
}

QMediaPlayer::MediaStatus MediaPlayerControl::mediaStatus() const {
    return QMediaPlayer::UnknownMediaStatus;
}

qint64 MediaPlayerControl::duration() const {
    return 0;
}

qint64 MediaPlayerControl::position() const {
    return 0;
}

void MediaPlayerControl::setPosition(qint64 position) {
}

int MediaPlayerControl::volume() const {
    return 0;
}

void MediaPlayerControl::setVolume(int volume) {
}

bool MediaPlayerControl::isMuted() const {
    return false;
}

void MediaPlayerControl::setMuted(bool muted) {
}

int MediaPlayerControl::bufferStatus() const {
    return 0;
}

bool MediaPlayerControl::isAudioAvailable() const {
    return false;
}

bool MediaPlayerControl::isVideoAvailable() const {
    return false;
}

bool MediaPlayerControl::isSeekable() const {
    return false;
}

QMediaTimeRange MediaPlayerControl::availablePlaybackRanges() const {
    return QMediaTimeRange();
}

qreal MediaPlayerControl::playbackRate() const {
    return 1.0;
}

void MediaPlayerControl::setPlaybackRate(qreal rate) {
}

QMediaContent MediaPlayerControl::media() const {
    return QMediaContent();
}

const QIODevice * MediaPlayerControl::mediaStream() const {
    return nullptr;
}

void MediaPlayerControl::setMedia(const QMediaContent & media, QIODevice *) {
    _player.load(media.canonicalUrl().url().toUtf8().constData());
}

void MediaPlayerControl::play() {
    qInfo("[%d]: MediaPlayerControl::play...", _id);
    _player.play();
}

void MediaPlayerControl::pause() {
    qInfo("[%d]: MediaPlayerControl::pause...", _id);
   _player.pause();
}

void MediaPlayerControl::stop() {
    qInfo("[%d]: MediaPlayerControl::stop...", _id);
}
