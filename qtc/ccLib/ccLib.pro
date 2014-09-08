#-------------------------------------------------
#
# Project created by QtCreator 2014-07-25T10:30:42
#
#-------------------------------------------------

QT       += widgets xml xmlpatterns

TARGET = ccLib
TEMPLATE = lib
CONFIG += staticlib
QMAKE_CXXFLAGS += -std=c++11 -Wno-unused-local-typedefs

INCLUDEPATH += ../../Src /home/sam/src/boost_1_56_0 /home/sam/src/libs/loki/include

SOURCES += \
    ../../Src/frmSelectEncoding.cpp \
    ../../Src/frSelectEncoding.cpp \
    ../../Src/qqOpenDlg.cpp \
    ../../Src/QtApp.cpp \
    ../../Src/QtSingletons.cpp \
    ../../Src/BomUtils.cpp \
    ../../Src/chasewidget.cpp \
    ../../Src/UntitledSequence.cpp \
    ../../Src/smException.cpp

HEADERS += \
    ../../Src/frmSelectEncoding.h \
    ../../Src/frSelectEncoding.h \
    ../../Src/qqOpenDlg.h \
    ../../Src/QtApp.h \
    ../../Src/QtSingletons.h \
    ../../Src/BomUtils.h \
    ../../Src/chasewidget.h \
    ../../Src/UntitledSequence.h \
    ../../Src/smException.h
unix {
    target.path = /usr/lib
    INSTALLS += target
}

FORMS += \
    ../../Src/Forms/frmSelectEncoding.ui \
    ../../Src/Forms/frSelectEncoding.ui
