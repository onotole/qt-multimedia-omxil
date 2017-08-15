#ifndef __VIDEO_WINDOW_CONTROL_H__
#define __VIDEO_WINDOW_CONTROL_H__

#include <QVideoWindowControl>
#include "../player/player.h"

class VideoWindowControl : public QVideoWindowControl {
    Q_OBJECT
public:
    explicit VideoWindowControl(QObject * parent = nullptr);

    void setPlayer(Player * player) {
        qInfo("[%d]: setPlayer = %p", _id, player);
        _player = player;
    }

    WId winId() const override;
    void setWinId(WId id) override;
    QRect displayRect() const override;
    void setDisplayRect(const QRect &rect) override;
    bool isFullScreen() const override;
    void setFullScreen(bool fullScreen) override;
    void repaint() override;
    QSize nativeSize() const override;
    Qt::AspectRatioMode aspectRatioMode() const override;
    void setAspectRatioMode(Qt::AspectRatioMode mode) override;
    int brightness() const override;
    void setBrightness(int brightness) override;
    int contrast() const override;
    void setContrast(int contrast) override;
    int hue() const override;
    void setHue(int hue) override;
    int saturation() const override;
    void setSaturation(int saturation) override;

private:
    static int _next_id;
    int _id;
    Player * _player;
};

#endif // __VIDEO_WINDOW_CONTROL_H__
