
#include <QVideoWindowControl>

#include "media_service.h"
#include "media_player_control.h"
#include "video_window_control.h"

MediaService::MediaService(QObject * parent) : QMediaService(parent) {}

QMediaControl * MediaService::requestControl(const char * name) {
    if (qstrcmp(name, QMediaPlayerControl_iid) == 0) {
        return static_cast<QMediaControl *>(new MediaPlayerControl(this));
    }

    if (qstrcmp(name, QVideoWindowControl_iid) == 0) {
        return static_cast<QMediaControl *>(new VideoWindowControl(this));
    }

    qWarning("MediaService: unsupported control: %s", qPrintable(name));
    return nullptr;
}

void MediaService::releaseControl(QMediaControl *) {}
