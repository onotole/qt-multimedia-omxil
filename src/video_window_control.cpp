#include "video_window_control.h"

VideoWindowControl::VideoWindowControl(QObject * parent) : QVideoWindowControl(parent) {
    qInfo("VideoWindowControl ctor...");
}

WId VideoWindowControl::winId() const {
    return WId();
}

void VideoWindowControl::setWinId(WId id) {
    qInfo("VideoWindowControl set win id...");
}

QRect VideoWindowControl::displayRect() const {
    return QRect();
}

void VideoWindowControl::setDisplayRect(const QRect &rect) {
}

bool VideoWindowControl::isFullScreen() const {

}

void VideoWindowControl::setFullScreen(bool fullScreen) {

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
