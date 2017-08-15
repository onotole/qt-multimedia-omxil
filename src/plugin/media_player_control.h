#ifndef __MEDIA_PLAYER_CONTROL_H__
#define __MEDIA_PLAYER_CONTROL_H__

#include <QMediaPlayerControl>
#include "../player/player.h"

class MediaPlayerControl : public QMediaPlayerControl {
    Q_OBJECT
public:
    explicit MediaPlayerControl(QObject * parent = nullptr);
    ~MediaPlayerControl();

    Player * player() { return &_player; }

    QMediaPlayer::State state() const override;
    QMediaPlayer::MediaStatus mediaStatus() const override;
    qint64 duration() const override;
    qint64 position() const override;
    void setPosition(qint64 position) override;
    int volume() const override;
    void setVolume(int volume) override;
    bool isMuted() const override;
    void setMuted(bool muted) override;
    int bufferStatus() const override;
    bool isAudioAvailable() const override;
    bool isVideoAvailable() const override;
    bool isSeekable() const override;
    QMediaTimeRange availablePlaybackRanges() const override;
    qreal playbackRate() const override;
    void setPlaybackRate(qreal rate) override;
    QMediaContent media() const override;
    const QIODevice *mediaStream() const override;
    void setMedia(const QMediaContent &media, QIODevice *stream) override;
    void play() override;
    void pause() override;
    void stop() override;

private:
    static int _next_id;
    int _id;
    Player _player;
};

#endif // __MEDIA_PLAYER_CONTROL_H__
