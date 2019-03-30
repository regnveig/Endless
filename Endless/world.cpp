#include "world.h"

World::World(QObject *parent, skylight newSun, skylight newMoon, weather newWeather) : QObject(parent),
    Sun(new skylight(newSun)),
    Moon(new skylight(newMoon)),
    Weather(new weather(newWeather)) { World::Play(); }

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

    Weather->id = startTimer(1000u);
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
        emit NewWeather(*Weather);
        if (Weather->time_sec == 0) {

            Weather->time_sec = 5000;               // or other
            Weather->type = weather_type::Rain;     // or other! Использовать глобальный генератор?
        }
    }
}

polar World::PolarSun() {

    qreal new_azimuth = QUIntToRad(SUN_DEVIATION_COE + quint16(SUN_AMPLITUDE * qSin(QUIntToRad(Sun->day))));
    qreal new_zenith = QUIntToRad(Sun->time);

    // Доработать алгоритм, чтобы зимой дни были короче

    return {new_azimuth, new_zenith};
}

polar World::PolarMoon() {

    qreal new_zenith = QUIntToRad(Moon->time);
    qreal new_azimuth = QUIntToRad(MOON_DEVIATION_COE + quint16(MOON_AMPLITUDE * qSin(QUIntToRad(Moon->day))));

    return {new_azimuth, new_zenith};
}

qreal World::LunarPhase() {

    polar new_sun = PolarSun();
    polar new_moon = PolarMoon();

    cartesius c_sun = {SUNMOON_COE * qSin(new_sun.zenith) * qCos(new_sun.azimuth),
                       SUNMOON_COE * qSin(new_sun.zenith) * qSin(new_sun.azimuth),
                       SUNMOON_COE * qCos(new_sun.zenith)};

    cartesius c_moon = {qSin(new_moon.zenith) * qCos(new_moon.azimuth),
                        qSin(new_moon.zenith) * qSin(new_moon.azimuth),
                        qCos(new_moon.zenith)};

    cartesius d_sun = {(c_sun.x - c_moon.x),
                       (c_sun.y - c_moon.y),
                       (c_sun.z - c_moon.z)};

    qreal length = qSqrt(qPow(d_sun.x, 2) + qPow(d_sun.y, 2) + qPow(d_sun.z, 2));

    return qAcos((0 - (c_moon.x * d_sun.x) - (c_moon.y * d_sun.y) - (c_moon.z * d_sun.z)) / length);

}
