TEMPLATE = app
TARGET = omx-player

CONFIG += link_pkgconfig exceptions

cross_compile {
  INCLUDEPATH += $$[QT_SYSROOT]/usr/include/IL
  PKGCONFIG += bcm_host
  LIBS += ../player/libplayer.a
  LIBS += -lopenmaxil
  DEFINES += OMX_RPI OMX_SKIP64BIT
} else {
  LIBS += ../player/libplayer.a
  PKGCONFIG += libomxil-bellagio
}


PKGCONFIG += libavcodec libavformat

SOURCES += $$files(*.cpp)
