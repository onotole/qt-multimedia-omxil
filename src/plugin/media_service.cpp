
#include <QVideoWindowControl>
#include <queue>

#include "media_service.h"
#include "media_player_control.h"
#include "video_window_control.h"

MediaService::MediaService(QObject * parent) : QMediaService(parent) {}

QMediaControl * MediaService::requestControl(const char * name) {
    static std::queue<Player *> players;

    if (qstrcmp(name, QMediaPlayerControl_iid) == 0) {
        qInfo("query media player control...");
        auto mpc = new MediaPlayerControl(this);
        players.push(mpc->player());
        return static_cast<QMediaControl *>(mpc);
    }

    if (qstrcmp(name, QVideoWindowControl_iid) == 0) {
        qInfo("query video window control...");
        auto vwc = new VideoWindowControl(this);
        vwc->setPlayer(players.front());
        players.pop();
        return static_cast<QMediaControl *>(vwc);
    }

    qWarning("MediaService: unsupported control: %s", qPrintable(name));
    return nullptr;
}

void MediaService::releaseControl(QMediaControl *) {
    qInfo("release control...");
}
