#-------------------------------------------------
#
# Project created by QtCreator 2019-03-30T02:42:05
#
#-------------------------------------------------

QT       += core gui widgets opengl

TARGET = Endless
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

# DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000

CONFIG += c++17

SOURCES += \
    global.cpp \
        main.cpp \
    gui_opengl.cpp \
    sky.cpp \
    weather.cpp

HEADERS += \
    gui_opengl.h \
    sky.h \
    global.h \
    weather.h

qnx: target.path = /dev/my/MyDosc/temp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
