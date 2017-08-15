#include "video_window_control.h"

int VideoWindowControl::_next_id = 1;

VideoWindowControl::VideoWindowControl(QObject * parent) : QVideoWindowControl(parent), _id(_next_id++) {
    qInfo("[%d]: VideoWindowControl ctor...", _id);
}

WId VideoWindowControl::winId() const {
    return WId();
}

void VideoWindowControl::setWinId(WId id) {
    qInfo("[%d]: VideoWindowControl set win id...", _id);
}

QRect VideoWindowControl::displayRect() const {
    return QRect();
}

void VideoWindowControl::setDisplayRect(const QRect & rc) {
    if (_player) {
        qInfo("[%d]: VideoWindowControl setDisplayRect...", _id);
        _player->set_output_rect({rc.x(), rc.y(), rc.width(), rc.height()});
    } else {
        qWarning("[%d]: Misfiring VideoWindowControl setDisplayRect: no player assigned!", _id);
    }
}

bool VideoWindowControl::isFullScreen() const {

}

void VideoWindowControl::setFullScreen(bool fullScreen) {
    qInfo("VideoWindowControl setFullscreen...");
}

void VideoWindowControl::repaint() {

}

QSize VideoWindowControl::nativeSize() const {
    return QSize();
}

Qt::AspectRatioMode VideoWindowControl::aspectRatioMode() const {
    return Qt::IgnoreAspectRatio;
}

void VideoWindowControl::setAspectRatioMode(Qt::AspectRatioMode mode) {
    qInfo("[%d]: VideoWindowControl setAspectRatioMode...", _id);
}

int VideoWindowControl::brightness() const {
    return 0;
}

void VideoWindowControl::setBrightness(int brightness) {
}

int VideoWindowControl::contrast() const {
    return 0;
}

void VideoWindowControl::setContrast(int contrast) {

}

int VideoWindowControl::hue() const {
    return 0;
}


void VideoWindowControl::setHue(int hue) {

}

int VideoWindowControl::saturation() const {
    return 0;
}

void VideoWindowControl::setSaturation(int saturation) {
}
