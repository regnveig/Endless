#include "global.h"
#include "gui_opengl.h"
#include "sky.h"
#include "weather.h"

int main(int argc, char *argv[]) {

    QApplication a(argc, argv);
    GUI_OpenGL w;
    Sky *sky = new Sky();
    Weather *weather = new Weather();
    w.show();

    a.connect(sky, SIGNAL(CelestialData(QList<celestial_data>)), &w, SLOT(CelestialData(QList<celestial_data>)));
    a.connect(sky, SIGNAL(GroundData(ground_data)), weather, SLOT(GroundData(ground_data)));
    a.connect(sky, SIGNAL(StarsData(QList<star>)), &w, SLOT(StarsData(QList<star>)));
    a.connect(weather, SIGNAL(WeatherData(QList<weather_data>)), &w, SLOT(WeatherData(QList<weather_data>)));

    return a.exec();
}
