TARGET = qtmedia_omxil_video

QT += multimedia gui
CONFIG += link_pkgconfig exceptions

cross_compile {
  INCLUDEPATH += $$[QT_SYSROOT]/usr/include/IL
  PKGCONFIG += bcm_host
  LIBS += ../player/libplayer.a
  LIBS += -lopenmaxil
  DEFINES += OMX_RPI
} else {
  LIBS += ../player/libplayer.a
  PKGCONFIG += libomxil-bellagio
}

PKGCONFIG += libavcodec libavformat

PLUGIN_TYPE = mediaservice
PLUGIN_CLASS_NAME = OMXVideoPlugin
load(qt_plugin)

HEADERS += $$files(*.h)
SOURCES += $$files(*.cpp)

OTHER_FILES += plugin_meta.json

