#include "gui_opengl.h"
#include <sky.h>
#include <QApplication>
#include <QDebug>

int main(int argc, char *argv[]) {

    //QApplication a(argc, argv);
    //GUI_OpenGL w;
    //w.show();

    QObject *object = new QObject(nullptr);
    Sky *sky = new Sky(object);
    Getter *getter = new Getter(object);

    object->connect(sky, SIGNAL(Data(QList<celestial_data>)), getter, SLOT(GetData(QList<celestial_data>)));
    sky->Loop();

    delete object;
    return 0;
    //return a.exec();
}
