#include "weather.h"

/* ------ CYCLONE SUBCLASS ------ */

Cyclone::Cyclone(qint8 new_strength, QVector3D new_eye_coord, quint32 new_life_time) {

    *strength = new_strength;
    *eye_coord = new_eye_coord;
    *life_time = new_life_time;
}

Cyclone::~Cyclone() {

    delete strength; strength = nullptr;
    delete eye_coord; eye_coord = nullptr;
    delete life_time; life_time = nullptr;
}

bool Cyclone::Tick(QVector3D summar_power) {

    *eye_coord += summar_power;
    eye_coord->normalize();

    (*life_time)--;
    return (*life_time <= 0);
}

qint8 Cyclone::getStrength() { return *strength; }

QVector3D Cyclone::getEyeCoord() { return *eye_coord; }

/* ------ WEATHER CLASS ------ */

Weather::Weather(QObject *parent) : QObject(parent) {

    Play(true);
}

Weather::~Weather() {

    Play(false);
}

void Weather::Play(bool play) {

    if (play) *Timer_ID = startTimer(100);
    else killTimer(*Timer_ID);
}

void Weather::SkyData(QList<celestial_data> data) {

    for (auto item = 0; item < data.size(); item++)
        if (data.at(item).name == QString("sun"))
            *Z = data.at(item).vect1.z();
}

void Weather::timerEvent([[maybe_unused]] QTimerEvent *event) {


}
