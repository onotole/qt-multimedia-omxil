TEMPLATE = lib
CONFIG += staticlib exceptions

cross_compile {
  INCLUDEPATH += $$[QT_SYSROOT]/usr/include/IL
  LIBS += -lopenmaxil
  DEFINES += OMX_RPI OMX_SKIP64BIT
}

HEADERS += $$files(src/*.h)
SOURCES += $$files(src/*.cpp)

HEADERS += $$files(omx/*.h)
SOURCES += $$files(omx/*.cpp)

HEADERS += $$files(sink/*.h)
SOURCES += $$files(sink/*.cpp)

HEADERS += $$files(*.h)
SOURCES += $$files(*.cpp)

