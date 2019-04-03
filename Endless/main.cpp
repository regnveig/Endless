#include "gui_opengl.h"
#include <sky.h>
#include <QApplication>
#include <QDebug>

int main(int argc, char *argv[]) {

    QApplication a(argc, argv);
    GUI_OpenGL w;
    w.show();

    return a.exec();
}
