#include "weather.h"

Weather::Weather(QObject *parent) : QObject(parent) {

}

Weather::~Weather() {

}

void Weather::Play(bool play) {

}

void Weather::SkyData(QList<celestial_data> data) {

    for (auto item = 0; item < data.size(); item++)
        if (data.at(item).name == QString("sun"))
            Z = data.at(item).vect1.z();
}

void Weather::timerEvent(QTimerEvent *event) {

}
