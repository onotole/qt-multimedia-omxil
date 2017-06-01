TARGET = qtmedia_omxil_video
 
QT += multimedia gui
CONFIG += link_pkgconfig exceptions

PLUGIN_TYPE = mediaservice
PLUGIN_CLASS_NAME = OMXVideoPlugin
load(qt_plugin)
 
SOURCES += $$files(src/*.cpp)
HEADERS += $$files(src/*.h)
OTHER_FILES += plugin_meta.json

