#include "weather.h"

Cyclone::Cyclone(quint64 new_lifetime, quint64 new_time_passed, QVector3D new_place, QVector3D new_speed, bool new_isCyclone) : lifetime(new_lifetime), time_passed(new_time_passed), place(new_place), speed(new_speed), isCyclone(new_isCyclone) { }

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

Weather::Weather(QFileInfo saved_file, QObject *parent) : QObject(parent) {

    // Open DB

    saved_db.setDatabaseName(saved_file.absoluteFilePath());

    if (!saved_db.open()) {
        qCritical() << "Weather: " << saved_db.lastError().text();
        throw QException();
    }

    QSqlQuery query(saved_db);

    // Timers

    if (!query.exec("SELECT * FROM endl3ss_weather_timers")) {
        qCritical() << "Weather: " << saved_db.lastError().text();
        throw QException();
    }

    query.next();
    Timer_interval = query.value(0).toInt();
    CycloneTimer_interval = query.value(1).toInt();
    Saver_interval = query.value(2).toInt();

    // Create Cyclone

    if (!query.exec("SELECT * FROM endl3ss_weather_cyclone")) {
        qCritical() <<  "Weather: " << saved_db.lastError().text();
        throw QException();
    }

    while (query.next()) {

        Cyclone * new_cyclone = new Cyclone(quint64(query.value(0).toInt()),
                                            quint64(query.value(1).toInt()),
                                            QVector3D(query.value(2).toFloat(), query.value(3).toFloat(), query.value(4).toFloat()),
                                            QVector3D(query.value(5).toFloat(), query.value(6).toFloat(), query.value(7).toFloat()),
                                            query.value(1).toBool());

        CyclonePack.append(new_cyclone);
    }

    // Season

    if (!query.exec("SELECT * FROM endl3ss_weather_other")) {
        qCritical() << "Weather: " << saved_db.lastError().text();
        throw QException();
    }

    query.next();
    season = qreal(query.value(0).toDouble());

    // Create Spectator

    if (!query.exec("SELECT * FROM endl3ss_weather_spectator")) {
        qCritical() << "Weather: " << saved_db.lastError().text();
        throw QException();
    }

    query.next();

    ground = query.value(0).toString();
    longitude = qreal(query.value(1).toDouble());
    latitude = qreal(query.value(2).toDouble());

    Play(true);

}

Weather::~Weather() {
    Play(false);
    for (auto &item : CyclonePack)
        delete item;
    saved_db.close();
}

void Weather::Play(bool play) {

    if (play) {
        Timer_ID = startTimer(Timer_interval);
        Saver_ID = startTimer(Saver_interval);
        CycloneTimer_ID = startTimer(CycloneTimer_interval);
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

    QSqlQuery query(saved_db);

    QString q = "DELETE FROM endl3ss_weather_cyclone";
    if (!query.exec(q)) qDebug() << query.lastError().text() << "(" << q << ")";

    if (!CyclonePack.empty()) {
    q = "INSERT INTO endl3ss_weather_cyclone VALUES ";
    for (auto &item: CyclonePack) {
        q += "( " + QString::number(item->getLifetime()) +
                ", " + QString::number(item->getTimePassed()) +
                ", " + QString::number(double(item->getPlace().x())) +
                ", " + QString::number(double(item->getPlace().y())) +
                ", " + QString::number(double(item->getPlace().z())) +
                ", " + QString::number(double(item->getSpeed().x())) +
                ", " + QString::number(double(item->getSpeed().y())) +
                ", " + QString::number(double(item->getSpeed().z())) +
                ", " + (item->getIsCyclone() ? "TRUE" : "FALSE") + "), ";

    }

    q.chop(2);
    q += ";";

    if (!query.exec(q)) qCritical() << "Weather: " << query.lastError().text() << "(" << q << ")";
}
    q = "UPDATE endl3ss_weather_spectator SET ground = '" + ground + "', "
                "longitude = " + QString::number(longitude) + ", latitude = " + QString::number(latitude) + ";";
    if (!query.exec(q)) qCritical() << "Weather: " << query.lastError().text() << "(" << q << ")";

    q = "UPDATE endl3ss_weather_other SET season = " + QString::number(season) + ";";
    if (!query.exec(q)) qCritical() << "Weather: " << query.lastError().text() << "(" << q << ")";

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

        *cyclone = Cyclone(new_lifetime, 0, new_vect, QVector3D(0.0f, 0.0f, 0.0f), !positive);
        return true;
    }

    return false;
}

void Weather::LoopCyclone() {

    Cyclone *cyclone = new Cyclone(0, 0, QVector3D(0.0f, 0.0f, 0.0f), QVector3D(0.0f, 0.0f, 0.0f), true);
    bool did = newCyclone(cyclone);
    if (did) CyclonePack.append(cyclone);

    for (auto &item : CyclonePack) {

        QVector3D force_summ = QVector3D(0.0f, 0.0f, 0.0f);

        for (auto &item2 : CyclonePack)
            if (item != item2) {
                QVector3D plus = item->getPlace() - item2->getPlace();
                plus *= float(qAbs(MASS_LOWERING * item2->getLifetime() / item->getLifetime()));
                force_summ += plus;
            }

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
    }

    qDebug() << CyclonePack.size();

}
