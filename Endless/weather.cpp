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

    if (play) *Timer_ID = startTimer(1);
    else killTimer(*Timer_ID);
}

void Weather::GroundData(ground_data data) {

    *season = data.season;
    *latitude = data.latitude;
    *longitude = data.longitude;
}

bool Weather::newCyclone(Weather::Cyclone *cyclone, QRandomGenerator *rand) {

    bool positive = rand->generate() > quint32(QRandomGenerator::max() / 2);

    qreal   c_azimuth = qreal(rand->generate()) / QRandomGenerator::max() * 2 * M_PI,
            c_zenith  = qreal(rand->generate()) / QRandomGenerator::max() * M_PI,
            c_f       = qreal(rand->generate()) / QRandomGenerator::max();

    qreal   c_latitude = M_PI_2 - c_zenith;

    qreal   c_surface     = 1.0;

    for (auto &item : *CyclonePack)
        c_surface -= 0.25 * item.getPower();
    if (c_surface < 0) c_surface = 0;

    qreal f_cyclone = Cyclone::RANDOM_COE * (qCos(c_latitude) * c_surface * qAbs(qSin(*season) * qSin(c_latitude)));
    qreal f_anticyclone = Cyclone::RANDOM_COE * (qCos(c_latitude) * c_surface * qAbs(qCos(*season) * qCos(c_latitude * 2)));

    quint32 new_lifetime = quint32(Cyclone::MIN_LIFETIME +
                                   (qreal(rand->generate()) / QRandomGenerator::max() *
                                   (Cyclone::MAX_LIFETIME - Cyclone::MIN_LIFETIME)));

    qreal new_power = qAbs(qSin(c_latitude)) * (positive ? 1.0 : -1.0); // в мелких циклонах power никогда не дойдёт до грозы.

    QVector3D new_vect = QVector3D(float(qSin(c_zenith) * qCos(c_azimuth)),
                                   float(qSin(c_zenith) * qSin(c_azimuth)),
                                   float(qCos(c_zenith)));

    if (((c_f < f_cyclone) && positive) || ((c_f < f_anticyclone) && (!positive))) {

        *cyclone = Cyclone(new_lifetime, new_power, new_vect);
        return true;
    }

    return false;
}

void Weather::Loop() {

    Cyclone *cyclone = new Cyclone(0, 0.0, QVector3D());
    bool did = newCyclone(cyclone, GLOBAL_RAND);
    if (did) CyclonePack->append(*cyclone);

    delete cyclone;
    cyclone = nullptr;

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

    qDebug() << CyclonePack->size();

    emit WeatherData(list);
}
