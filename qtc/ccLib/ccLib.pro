#-------------------------------------------------
#
# Project created by QtCreator 2014-07-25T10:30:42
#
#-------------------------------------------------

QT       += widgets xml xmlpatterns

TARGET = ccLib
TEMPLATE = lib
CONFIG += staticlib

unix {
    QMAKE_CXXFLAGS += -std=c++17 -Wno-unused-local-typedefs -Wno-deprecated-copy
}

include (user_config.pri)

DEFINES += FMT_HEADER_ONLY

INCLUDEPATH += ../../Src \
               ../../include \
               $$ROOT_BOOST \
               $$ROOT_LOKI/include \
               $$ROOT_FMT/include

win32 {
    INCLUDEPATH += $$ROOT_WTL\include
}

SOURCES += \
    ../../Src/BomUtils.cpp \
    ../../Src/UntitledSequence.cpp \
    ../../Src/cpp_lpstr.cpp \
    ../../Src/cpp_string.cpp \
    ../../Src/smException.cpp \
    ../../Src/Thread2.cpp


HEADERS += \
    ../../include/BomUtils.h \
    ../../include/UntitledSequence.h \
    ../../include/smException.h \
    ../../include/cpp_lpstr.h \
    ../../include/cpp_string.h \
    ../../include/cc_array.hpp \
    ../../include/cc_atomic.hpp \
    ../../include/cc_memory.hpp \
    ../../include/cpp_sstream.h \
    ../../include/Thread2.h \
    ../../Src/pre_cc.h \
    ../../include/predef_cc.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
