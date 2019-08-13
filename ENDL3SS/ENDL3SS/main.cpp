#include <QApplication>
#include "sky.h"
#include "gui_opengl.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QFileInfo saved("init.db");

    GUI_OpenGL w;
    Sky *sky = new Sky(saved, 10, 30000);
    w.show();

    a.connect(sky, SIGNAL(CelestialData(QList<CelestialInfo>)), &w, SLOT(CelestialData(QList<CelestialInfo>)));
    a.connect(sky, SIGNAL(StarsData(QList<StarInfo>)), &w, SLOT(StarsData(QList<StarInfo>)));

    return a.exec();
}
