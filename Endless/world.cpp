#include "world.h"

World::World(QObject *parent, skylight newSun, skylight newMoon, weather newWeather) : QObject(parent),
    Sun(new skylight(newSun)),
    Moon(new skylight(newMoon)),
    Weather(new weather(newWeather)) {

    World::Play();

}

World::~World() {

    World::Pause();

    delete Sun;     Sun = nullptr;
    delete Moon;    Moon = nullptr;
    delete Weather; Weather = nullptr;

}

void World::Play() {

    Sun->time_id = startTimer(SUNTIME_INTERVAL);
    Moon->time_id = startTimer(MOONTIME_INTERVAL);
    Sun->day_id = startTimer(SUNDAY_INTERVAL);
    Moon->day_id = startTimer(MOONDAY_INTERVAL);

    Weather->id = startTimer(WEATHER_INTERVAL);

}

void World::Pause() {

    killTimer(Sun->time_id);
    killTimer(Moon->time_id);
    killTimer(Sun->day_id);
    killTimer(Moon->day_id);

    killTimer(Weather->id);

}

void World::timerEvent(QTimerEvent *event) {

    int id = event->timerId();

    if (id == Sun->time_id) { Sun->time++; return; }
    if (id == Moon->time_id) { Moon->time++; return; }
    if (id == Sun->day_id) { Sun->day++; return; }
    if (id == Moon->day_id) { Moon->day++; return; }

    if (id == Weather->id) {

        Weather->time_sec--;

        if (Weather->time_sec == 0) {

            Weather->time_sec = 5000;               // or other
            Weather->type = weather_type::Rain;     // or other!

        }

    }


}

polar World::toPolar(skylight *SkyLight, quint16 Deviation, quint16 Amplitude) {
    quint16 newZenith = static_cast<quint16>(Amplitude * qSin((SkyLight->day) / std::numeric_limits<quint64>::max() * (2 * M_PI)) + Deviation);
    return {SkyLight->time, newZenith};
}
