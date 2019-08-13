QT       += core gui widgets opengl sql

TARGET = Endless
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

# DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000

CONFIG += c++17
SOURCES += \
        gui_opengl.cpp \
        main.cpp \
        sky.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    global.h \
    gui_opengl.h \
    sky.h
