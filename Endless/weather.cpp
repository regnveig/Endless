#include "weather.h"

/* ------ CYCLONE SUBCLASS ------ */

Weather::Cyclone::Cyclone(quint32 new_lifetime, qreal new_power, QVector3D new_vect) :
    lifetime(new_lifetime), power(new_power), vect(new_vect) { }

Weather::Cyclone::~Cyclone() { }

void Weather::Cyclone::Loop(QVector3D force_summ) {

    speed += force_summ;
    speed.normalize();
    speed /= 10;
    vect += speed;
    vect.normalize();
    xtime++;
}

bool Weather::Cyclone::Is() { return (xtime < lifetime); }

qreal Weather::Cyclone::getPower() { return (power * qSin(xtime * M_PI / lifetime)); }

QVector3D Weather::Cyclone::getVect() { return vect; }

/* ------ WEATHER CLASS ------ */

Weather::Weather(QObject *parent) : QObject(parent) { Play(true); }

Weather::~Weather() {

    Play(false);

    delete CyclonePack; CyclonePack = nullptr;

    delete Timer_ID; Timer_ID = nullptr;
    delete season; season = nullptr;
    delete longitude; longitude = nullptr;
    delete latitude; latitude = nullptr;
}

void Weather::timerEvent([[maybe_unused]] QTimerEvent *event) { Loop(); }

void Weather::Play(bool play) {

    if (play) *Timer_ID = startTimer(20);
    else killTimer(*Timer_ID);
}

void Weather::GroundData(ground_data data) {

    *season = data.season;
    *latitude = data.latitude;
    *longitude = data.longitude;
}

void Weather::Loop() {

    qreal   c_azimuth,
            c_zenith,
            c_f;

    qreal   c_latitude;
    qreal   c_surface     = 1.0;

    for (auto &item : *CyclonePack)
        c_surface -= 0.25 * item.getPower();
    if (c_surface < 0) c_surface = 0;

    c_azimuth = qreal(GLOBAL_RAND.generate()) / QRandomGenerator::max() * 2 * M_PI;
    c_zenith  = qreal(GLOBAL_RAND.generate()) / QRandomGenerator::max() * M_PI;
    c_f       = qreal(GLOBAL_RAND.generate()) / QRandomGenerator::max();

    c_latitude = M_PI_2 - c_zenith;

    if (c_f < 0.1 * (qCos(c_latitude) * c_surface * qAbs(qSin(*season) * qSin(c_latitude))))
        CyclonePack->append(Cyclone(quint32(qreal(GLOBAL_RAND.generate()) / QRandomGenerator::max() * 1024),
                                    qAbs(qSin(c_latitude) * 0.5),
                                    QVector3D(float(qSin(c_zenith) * qCos(c_azimuth)),
                                              float(qSin(c_zenith) * qSin(c_azimuth)),
                                              float(qCos(c_zenith)))));

    c_azimuth = qreal(GLOBAL_RAND.generate()) / QRandomGenerator::max() * 2 * M_PI;
    c_zenith  = qreal(GLOBAL_RAND.generate()) / QRandomGenerator::max() * M_PI;
    c_f       = qreal(GLOBAL_RAND.generate()) / QRandomGenerator::max();

    c_latitude = M_PI_2 - c_zenith;

    if (c_f < 0.1 * (qCos(c_latitude) * c_surface * qAbs(qCos(*season) * qCos(c_latitude * 2))))
        CyclonePack->append(Cyclone(quint32(qreal(GLOBAL_RAND.generate()) / QRandomGenerator::max() * 1024),
                                    - qAbs(qSin(c_latitude) * 0.5),
                                    QVector3D(float(qSin(c_zenith) * qCos(c_azimuth)),
                                              float(qSin(c_zenith) * qSin(c_azimuth)),
                                              float(qCos(c_zenith)))));

    for (auto &item : *CyclonePack) {

        QVector3D force_summ = QVector3D(0.0f, 0.0f, 0.0f);

        for (auto &item2 : *CyclonePack) {

            QVector3D v = item.getVect() - item2.getVect();
            if (v.length() != 0.0f) v = v / v.length(); // Ввести компонент "веса" циклона
            force_summ += v;
        }
        item.Loop(force_summ);
    }

    // Сборщик мусора

    QMutableListIterator<Cyclone> i(*CyclonePack);
    while (i.hasNext()) if (!i.next().Is()) i.remove();

    QList<weather_data> list;

    for (auto &item : *CyclonePack)
        list.append({item.getVect(), item.getPower()});

    emit WeatherData(list);
}
