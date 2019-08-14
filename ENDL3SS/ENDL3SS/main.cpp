#include <QApplication>
#include "sky.h"
#include "gui_opengl.h"
#include "console.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QFileInfo saved("./save/sky.module");

    GUI_OpenGL w;
    Sky *sky = new Sky(saved, 1, 30000);
    w.show();

    Console console;
    console.show();

    a.connect(sky, SIGNAL(CelestialData(QList<CelestialInfo>)), &w, SLOT(CelestialData(QList<CelestialInfo>)));
    a.connect(sky, SIGNAL(StarsData(QList<StarInfo>)), &w, SLOT(StarsData(QList<StarInfo>)));

    a.connect(sky, SIGNAL(ConsoleOutput(QString)), &console, SLOT(ConsoleOutput(QString)));
    a.connect(&console, SIGNAL(SkyConsole(QString, QString)), sky, SLOT(Console(QString, QString)));

    return a.exec();
}
