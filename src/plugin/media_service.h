#ifndef __MEDIA_SERVICE_H__
#define __MEDIA_SERVICE_H__

#include <QMediaService>

class MediaPlayerControl;
class VideoWindowControl;

class MediaService : public QMediaService {
    Q_OBJECT
public:
    explicit MediaService(QObject * parent = nullptr);
    QMediaControl * requestControl(const char * name);
    void releaseControl(QMediaControl * control);
};

#endif // __MEDIA_SERVICE_H__
