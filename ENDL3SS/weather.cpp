#include "weather.h"

Cyclone::Cyclone(quint64 new_lifetime, QVector3D new_place, bool new_isCyclone) : lifetime(new_lifetime), place(new_place), isCyclone(new_isCyclone) { }

void Cyclone::Loop(QVector3D force_summ) {

    speed += force_summ;
    speed.normalize();
    speed /= SPEED_LOWERING;
    place += speed;
    place.normalize();
    time_passed++;
}

MatrixCell Cyclone::getMatrixCellByPoint(QVector3D spectator) {

    qreal s = qAcos(qreal(QVector3D::dotProduct(spectator, place))) * M_PI;

    qreal point_power = getPower() * qPow(2, - (s * POWER_LOWERING / lifetime));

    QVector3D point_wind = QVector3D::crossProduct(spectator, place);
    point_wind.normalize();
    point_wind *= float(point_power) * WIND_LOWERING;

    return {point_power, point_wind};
}

qreal Cyclone::getPower() { return qSin(time_passed * M_PI / lifetime) * (isCyclone ? -1 : 1); }

QVector3D Cyclone::getPlace() { return place; }

bool Cyclone::exists() { return (time_passed < lifetime); }

Weather::Weather(QFileInfo saved_file, int TimerInterval, int SaverInterval, QObject *parent) : QObject(parent), Timer_interval(TimerInterval), Saver_interval(SaverInterval) {

    // Open DB

    saved_db.setDatabaseName(saved_file.absoluteFilePath());

    if (!saved_db.open()) {
        emit ConsoleOutput(saved_db.lastError().text());
        throw QException();
    }

    QSqlQuery query;

    Play(true);

}

Weather::~Weather() {
    Play(false);
}

void Weather::Play(bool play) {

    if (play) {
        Timer_ID = startTimer(Timer_interval);
        Saver_ID = startTimer(Saver_interval);
        CycloneTimer_ID = startTimer(CycloneTimer_Interval);
    } else {
        killTimer(Timer_ID);
        killTimer(Saver_ID);
        killTimer(CycloneTimer_ID);
    }
}

void Weather::timerEvent([[maybe_unused]] QTimerEvent *event) {

    if (event->timerId() == Timer_ID) {

    }

    if (event->timerId() == CycloneTimer_ID) LoopCyclone();
    if (event->timerId() == Saver_ID) Save();

}

void Weather::Save() {

    QSqlQuery query;

}

void Weather::Console(QString var, QString value) {

}

void Weather::CelestialData(QList<CelestialInfo> data) {
    for (auto item = 0; item < data.size(); item++)
        if (data.at(item).name == ground)
            season = data.at(item).season;
}

bool Weather::newCyclone(Cyclone *cyclone) {

    bool positive = rand.generate() > quint32(QRandomGenerator::max() / 2);

    qreal   c_azimuth = qreal(rand.generate()) / QRandomGenerator::max() * 2 * M_PI,
            c_zenith  = qreal(rand.generate()) / QRandomGenerator::max() * M_PI,
            c_f       = qreal(rand.generate()) / QRandomGenerator::max();

    qreal   c_latitude = M_PI_2 - c_zenith;

    qreal   c_surface     = 1.0;

    for (auto &item : CyclonePack)
        c_surface -= 0.25 * item->getPower();
    if (c_surface < 0) c_surface = 0;

    qreal f_cyclone = RANDOM_COE * (qCos(c_latitude) * c_surface * qAbs(qSin(season) * qSin(c_latitude)));
    qreal f_anticyclone = RANDOM_COE * (qCos(c_latitude) * c_surface * qAbs(qCos(season) * qCos(c_latitude * 2)));

    quint32 new_lifetime = quint32(MIN_LIFETIME +
                                   (qreal(rand.generate()) / QRandomGenerator::max() *
                                   (MAX_LIFETIME - MIN_LIFETIME)));

    QVector3D new_vect = QVector3D(float(qSin(c_zenith) * qCos(c_azimuth)),
                                   float(qSin(c_zenith) * qSin(c_azimuth)),
                                   float(qCos(c_zenith)));

    if (((c_f < f_cyclone) && positive) || ((c_f < f_anticyclone) && (!positive))) {

        *cyclone = Cyclone(new_lifetime, new_vect, !positive);
        return true;
    }

    return false;
}

void Weather::LoopCyclone() {

    Cyclone *cyclone = new Cyclone(0, QVector3D(), true);
    bool did = newCyclone(cyclone);
    if (did) CyclonePack.append(cyclone);

    for (auto &item : CyclonePack) {

        QVector3D force_summ = QVector3D(0.0f, 0.0f, 0.0f);

        for (auto &item2 : CyclonePack)
            force_summ += (item->getPlace() - item2->getPlace()) * float(qAbs(MASS_LOWERING * item2->getPower() / item->getPower()));

        item->Loop(force_summ);
    }

    // Сборщик мусора

    QMutableListIterator<Cyclone *> i(CyclonePack);
    while (i.hasNext()) {
        Cyclone * t = i.next();
        if (!t->exists()) {
            delete t;
            i.remove();
    }

    qDebug() << CyclonePack.size();

}
