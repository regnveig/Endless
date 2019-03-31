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

CONFIG += c++11

SOURCES += \
        main.cpp \
    world.cpp \
    gui_opengl.cpp

HEADERS += \
    world.h \
    gui_opengl.h

FORMS +=

qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
