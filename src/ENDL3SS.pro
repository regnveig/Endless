QT       += core gui widgets opengl sql

TARGET = ENDL3SS
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

CONFIG += c++17
SOURCES += \
        console.cpp \
        gui_opengl.cpp \
        main.cpp \
        sky.cpp \
        weather.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DESTDIR = $${_PRO_FILE_PWD_}/../bin

copydata.commands = $(COPY_DIR) $${_PRO_FILE_PWD_}/../init $$DESTDIR
first.depends = $(first) copydata
export(first.depends)
export(copydata.commands)
QMAKE_EXTRA_TARGETS += first copydata

HEADERS += \
    console.h \
    global.h \
    gui_opengl.h \
    sky.h \
    weather.h

FORMS += \
    console.ui
