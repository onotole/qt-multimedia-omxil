#include "plugin.h"
#include "media_service.h"

QMediaService* OMXVideoPlugin::create(QString const & key) {
    if (key == QLatin1String(Q_MEDIASERVICE_MEDIAPLAYER)) {
        MediaService * service = new MediaService;
        return static_cast<QMediaService *> (service);
    }
    qWarning("OMXVideoPlugin: unsupported key: %s", qPrintable(key));
    return nullptr;
}

void OMXVideoPlugin::release(QMediaService *service) {
    MediaService * s = static_cast<MediaService *>(service);
    delete s;
}

QMediaServiceProviderHint::Features OMXVideoPlugin::supportedFeatures(const QByteArray &service) const {
    if (service == Q_MEDIASERVICE_MEDIAPLAYER)
        return QMediaServiceProviderHint::VideoSurface;
    return QMediaServiceProviderHint::Features();
}

QList<QByteArray> OMXVideoPlugin::devices(const QByteArray &) const {
    return QList<QByteArray>() << "default";
}

QString OMXVideoPlugin::deviceDescription(const QByteArray &, const QByteArray &) {
    return QString();
}
