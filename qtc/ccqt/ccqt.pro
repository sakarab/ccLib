QT += widgets

TEMPLATE = lib
CONFIG += staticlib

unix {
    QMAKE_CXXFLAGS += -std=c++17 -Wno-unused-local-typedefs -Wno-deprecated-copy
}

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

include (user_config.pri)

INCLUDEPATH += ../../Src \
               ../../include \
               $$ROOT_BOOST \
               $$ROOT_LOKI/include \
               $$ROOT_FMT/include

SOURCES += \
    ../../Src/BomUtilsQT.cpp \
    ../../Src/frmSelectEncoding.cpp \
    ../../Src/frSelectEncoding.cpp \
    ../../Src/qqOpenDlg.cpp \
    ../../Src/QtApp.cpp \
    ../../Src/QtSingletons.cpp \
    ../../Src/chasewidget.cpp \
    ../../Src/qt_string.cpp

HEADERS += \
    ../../include/BomUtilsQT.h \
    ../../include/frmSelectEncoding.h \
    ../../include/frSelectEncoding.h \
    ../../include/qqOpenDlg.h \
    ../../include/QtApp.h \
    ../../include/QtSingletons.h \
    ../../include/chasewidget.h \
    ../../include/qt_string.h \
    ../../Src/pre_ccqt.h

FORMS += \
    ../../Src/Forms/frmSelectEncoding.ui \
    ../../Src/Forms/frSelectEncoding.ui

# Default rules for deployment.
unix {
    target.path = $$[QT_INSTALL_PLUGINS]/generic
}
!isEmpty(target.path): INSTALLS += target
