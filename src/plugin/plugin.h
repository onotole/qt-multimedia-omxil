#ifndef __PLUGIN_H__
#define __PLUGIN_H__

#include <QMediaServiceProviderPlugin>

#include "media_service.h"

class OMXVideoPlugin : public QMediaServiceProviderPlugin
        , public QMediaServiceSupportedDevicesInterface
        , public QMediaServiceFeaturesInterface {
    Q_OBJECT
    Q_INTERFACES (QMediaServiceSupportedDevicesInterface)
    Q_INTERFACES (QMediaServiceFeaturesInterface)
    Q_PLUGIN_METADATA (IID "org.qt-project.qt.mediaserviceproviderfactory/5.0" FILE "plugin_meta.json")

public:
    QMediaService* create(QString const& key) Q_DECL_OVERRIDE;
    void release(QMediaService *service) Q_DECL_OVERRIDE;

    QMediaServiceProviderHint::Features supportedFeatures(const QByteArray &service) const Q_DECL_OVERRIDE;

    QList<QByteArray> devices(const QByteArray &service) const;
    QString deviceDescription(const QByteArray &service, const QByteArray &device);
};

#endif // __PLUGIN_H__
