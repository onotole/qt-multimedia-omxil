TARGET = qtmedia_omxil_video
 
QT += multimedia gui
CONFIG += link_pkgconfig exceptions

PLUGIN_TYPE = mediaservice
PLUGIN_CLASS_NAME = OMXVideoPlugin
load(qt_plugin)

cross_compile {
  INCLUDEPATH += $$[QT_SYSROOT]/usr/include/IL
  LIBS += -lopenmaxil
  DEFINES += OMX_RPI
} else {
  PKGCONFIG += libomxil-bellagio
}

message($$INCLUDEPATH)
 
SOURCES += $$files(src/*.cpp)
SOURCES += $$files(src/omx/*.cpp)
HEADERS += $$files(src/*.h)
HEADERS += $$files(src/omx/*.h)
OTHER_FILES += plugin_meta.json

