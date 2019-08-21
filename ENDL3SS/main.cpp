#include <QApplication>
#include "sky.h"
#include "gui_opengl.h"
#include "console.h"
#include "weather.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QFileInfo saved_sky("./saved/sky.module");
    QFileInfo saved_weather("./saved/weather.module");

    GUI_OpenGL w;
    Sky *sky = new Sky(saved_sky);
    Weather *weather = new Weather(saved_weather);
    w.show();

    Console console;
    console.show();

    a.connect(sky, SIGNAL(CelestialData(QList<CelestialInfo>)), &w, SLOT(CelestialData(QList<CelestialInfo>)));
    a.connect(sky, SIGNAL(CelestialData(QList<CelestialInfo>)), weather, SLOT(CelestialData(QList<CelestialInfo>)));
    a.connect(sky, SIGNAL(StarsData(QList<StarInfo>)), &w, SLOT(StarsData(QList<StarInfo>)));

    a.connect(sky, SIGNAL(ConsoleOutput(QString)), &console, SLOT(ConsoleOutput(QString)));
    a.connect(&console, SIGNAL(SkyConsole(QString, QString)), sky, SLOT(Console(QString, QString)));
    // debug
    a.connect(weather, SIGNAL(WeatherData(QList<weather_data>)), &w, SLOT(WeatherData(QList<weather_data>)));
    // -----

    return a.exec();
}
