#include "sky.h"

bool Sky::CelestialSort(const CelestialInfo &a, const CelestialInfo &b) { return a.distance > b.distance; }

QVector3D Sky::MatrixRotate(QVector3D vect, qreal angle_x, qreal angle_y, qreal angle_z) {

    // Это древняя магия под названием "матрица поворота". Она просто работает, и неважно, как

    if (angle_x != 0.0)
        vect = QVector3D(vect.x(),
                         vect.y() * float(qCos(angle_x)) - vect.z() * float(qSin(angle_x)),
                         vect.y() * float(qSin(angle_x)) + vect.z() * float(qCos(angle_x)));

    if (angle_y != 0.0)
        vect = QVector3D(vect.x() * float(qCos(angle_y)) + vect.z() * float(qSin(angle_y)),
                         vect.y(),
                         -vect.x() * float(qSin(angle_y)) + vect.z() * float(qCos(angle_y)));

    if (angle_z != 0.0)
        vect = QVector3D(vect.x() * float(qCos(angle_z)) - vect.y() * float(qSin(angle_z)),
                         vect.x() * float(qSin(angle_z)) + vect.y() * float(qCos(angle_z)),
                         vect.z());
    return vect;
}

qreal Sky::Angle(qreal angle) {

    if (angle >= (2 * M_PI)) return (angle - (qFloor(angle / (2 * M_PI)) * 2 * M_PI));
    if (angle < 0) return (angle + (qCeil(qFabs(angle / (2 * M_PI))) * 2 * M_PI));
    return angle;
}

QVector3D Sky::SphericRandom(QRandomGenerator *rand) {

    bool trig = true;
    qreal azimuth = 0.0, zenith = 0.0, f = 0.0;

    while (trig) {

        azimuth = qreal(rand->generate()) / QRandomGenerator::max() * 2 * M_PI;
        zenith = qreal(rand->generate()) / QRandomGenerator::max() * M_PI;
        f = qreal(rand->generate()) / QRandomGenerator::max();

        if (f < qSin(zenith)) trig = false;
    }

    return QVector3D(float(qSin(zenith) * qCos(azimuth)),
                     float(qSin(zenith) * qSin(azimuth)),
                     float(qCos(zenith)));
}

Sky::Celestial::Celestial(Celestial * new_parent, QString new_id, float new_distance, float new_radius,
          qreal new_ecliptic, qreal new_axis, qreal new_speed,
          qreal new_rotation, qreal new_date, qreal new_time) : id(new_id),
    distance(new_distance),
    radius(new_radius), ecliptic(new_ecliptic), axis(new_axis),
    speed(new_speed), rotation(new_rotation), date(new_date),
    time(new_time) {

    parent = new_parent;
    if (parent != nullptr) parent->AddChild(this);
}

Sky::Celestial::~Celestial() {

    while(!children.isEmpty())
        delete children.takeFirst();
    parent = nullptr;
};

int Sky::Celestial::AddChild(Celestial *child) {

    for (auto &item : children)
        if (item == child) return 1;
    children.append(child);
    return 0;
}

QList<Sky::Celestial *> Sky::Celestial::getFamily() {

    QList<Celestial *> family = children;
    for (auto &item : children)
        family.append(item->getFamily());
    return family;
}

QVector3D Sky::Celestial::getPosition() {

    if (parent == nullptr)
        return {0.0, 0.0, 0.0};
    QVector3D d = QVector3D(float(qCos(date) * qCos(ecliptic)) * distance,
                            float(qSin(date) * qCos(ecliptic)) * distance,
                            float(qSin(date) * qSin(ecliptic)) * distance);

    QVector3D ppos = parent->getPosition();
    return {ppos + d};
}

void Sky::Celestial::LoopFamily() {

    for (auto &item : children)
        item->LoopFamily();

    if (parent == nullptr) return;

    date = Sky::Angle(date + speed);
    time = Sky::Angle(time + rotation);
}

Sky::Sky(QFileInfo saved_file, QObject *parent) : QObject(parent) {

    // Open DB

    saved_db.setDatabaseName(saved_file.absoluteFilePath());

    if (!saved_db.open()) {
        qCritical() << "Sky: " << saved_db.lastError().text();
        throw QException();
    }

    QSqlQuery query(saved_db);

    // Timers

    if (!query.exec("SELECT * FROM endl3ss_sky_timers")) {
        qCritical() << "Sky: " << saved_db.lastError().text();
        throw QException();
    }

    query.next();
    Timer_interval = query.value(0).toInt();
    Saver_interval = query.value(1).toInt();

    // Create Celestial

    if (!query.exec("SELECT * FROM endl3ss_sky_celestial")) {
        qCritical() << "Sky: " << saved_db.lastError().text();
        throw QException();
    }

    while (query.next()) {
        QString the_parent = query.value(1).toString();

        Celestial * pointer = nullptr;
        if (the_parent != "allah")
            for (auto &planet: CelestialPack)
                if (planet->getID() == the_parent) pointer = planet;

        Celestial * new_celestial = new Celestial(pointer,
                  query.value(0).toString(),
                  query.value(2).toFloat(),
                  query.value(3).toFloat(),
                  qreal(query.value(4).toDouble()),
                  qreal(query.value(5).toDouble()),
                  qreal(query.value(6).toDouble()),
                  qreal(query.value(7).toDouble()),
                  qreal(query.value(8).toDouble()),
                  qreal(query.value(9).toDouble()));

        CelestialPack.append(new_celestial);
    }

    // Create stars

    if (!query.exec("SELECT * FROM endl3ss_sky_stars")) {
        qCritical() << "Sky: " << saved_db.lastError().text();
        throw QException();
    }

    query.next();
    quint32 StarNum = quint32(query.value(0).toInt());
    quint8 StarType = 0;
    QRandomGenerator rand(quint32(query.value(1).toInt()));

    for (quint32 item = 0; item < StarNum; item++) {

        QVector3D vect = SphericRandom(&rand);
        if (item > StarNum * (1 - query.value(2).toDouble())) StarType = 1;
        if (item > StarNum * (1 - query.value(3).toDouble())) StarType = 2;
        StarPack.append({vect, StarType});
    }
    Stars_on = query.value(4).toInt() == 1 ? true : false;

    // Create Spectator

    if (!query.exec("SELECT * FROM endl3ss_sky_spectator")) {
        qCritical() << "Sky: " << saved_db.lastError().text();
        throw QException();
    }

    query.next();

    QString the_ground = query.value(0).toString();

    for (auto &planet: CelestialPack)
        if (planet->getID() == the_ground) ground = planet;

    longitude = qreal(query.value(1).toDouble());
    latitude = qreal(query.value(2).toDouble());

    Play(true);

}

Sky::~Sky() {
    Play(false);
    delete CelestialPack[0];
    saved_db.close();
}

Sky::System3D Sky::System() {

    qreal al = Angle(longitude + ground->getTime());
    float radius = ground->getRadius();
    qreal axis = ground->getAxis();

    QVector3D z (float(qCos(latitude) * qCos(al)) * radius,
                float(qCos(latitude) * qSin(al)) * radius,
                float(qSin(latitude)) * radius);

    QVector3D x (float(qCos(latitude - M_PI_2) * qCos(al)) * radius,
                float(qCos(latitude - M_PI_2) * qSin(al)) * radius,
                float(qSin(latitude - M_PI_2)) * radius);

    QVector3D y (float(qCos(al + M_PI_2)) * radius,
                float(qSin(al + M_PI_2)) * radius,
                0.0f);

    return {MatrixRotate(x, 0.0, axis, 0.0),
            MatrixRotate(y, 0.0, axis, 0.0),
            MatrixRotate(z, 0.0, axis, 0.0)};
}

void Sky::SendCelestial() {

    QList<CelestialInfo> list;
    System3D System_ = System();
    QVector3D Pos = (ground->getPosition() + System_.axis_z);

    QVector3D SunPos = CelestialPack[0]->getPosition() - Pos;

    for (auto &item : CelestialPack) {

        QString new_name = item->getID();
        QVector3D new_coord = item->getPosition() - Pos;
        float new_distance = new_coord.length();
        qreal new_angular_size =
            2 * qAtan(qreal(item->getRadius() / (2.0f * new_distance)));

        QVector3D new_vect1 (QVector3D::dotProduct(new_coord, System_.axis_x),
                            QVector3D::dotProduct(new_coord, System_.axis_y),
                            QVector3D::dotProduct(new_coord, System_.axis_z));
        new_vect1.normalize();

        QVector3D toSpec = QVector3D(0.0f, 0.0f, 0.0f) - new_coord;
        QVector3D toSun = SunPos - new_coord;
        qreal new_phase = 0.0;
        if (!toSun.isNull())
            new_phase = qAcos(qreal(QVector3D::dotProduct(toSpec, toSun) / (toSpec.length() * toSun.length())));

        qreal new_season = ground->getDate();
        list.append({new_name, new_vect1, new_distance, new_angular_size, new_phase, new_season});
    }

    std::sort(list.begin(), list.end(), CelestialSort);

    emit CelestialData(list);
}


void Sky::SendStars() {

    System3D System_ = System();
    QList<StarInfo> star_list;

    if (Stars_on) {
    for (auto &item : StarPack) {

        QVector3D new_vect (QVector3D::dotProduct(item.coord, System_.axis_x),
                           QVector3D::dotProduct(item.coord, System_.axis_y),
                           QVector3D::dotProduct(item.coord, System_.axis_z));
        new_vect.normalize();
        star_list.append({new_vect, item.type});
    }
    }

    emit StarsData(star_list);
}

void Sky::Play(bool play) {

    if (play) {
        Timer_ID = startTimer(Timer_interval);
        Saver_ID = startTimer(Saver_interval);
    } else {
        killTimer(Timer_ID);
        killTimer(Saver_ID);
    }
}

void Sky::timerEvent([[maybe_unused]] QTimerEvent *event) {

    if (event->timerId() == Timer_ID) {
        CelestialPack[0]->LoopFamily();
        SendCelestial();
        SendStars();
    }

    if (event->timerId() == Saver_ID) Save();

}

void Sky::Save() {

    QSqlQuery query(saved_db);
    for (auto item: CelestialPack) {
        QString q = "UPDATE endl3ss_sky_celestial SET date = " + QString::number(item->getDate()) + ", "
                    "time = " + QString::number(item->getTime()) + " WHERE id = '" + item->getID() + "'; ";
        if (!query.exec(q)) qCritical() << "Sky: " << query.lastError().text() << "(" << q << ")";
    }
    QString q = "UPDATE endl3ss_sky_spectator SET ground = '" + ground->getID() + "', "
                "longitude = " + QString::number(longitude) + ", latitude = " + QString::number(latitude) + ";";
    if (!query.exec(q)) qCritical() << "Sky: " << query.lastError().text() << "(" << q << ")";
    q = "UPDATE endl3ss_sky_stars SET stars_on = " + QString::number(Stars_on ? 1 : 0 ) + ";";
    if (!query.exec(q)) qCritical() << "Sky: " << query.lastError().text() << "(" << q << ")";

}

void Sky::Console(QString var, QString value) {

    if (var == "stars") {
        if (value == "on") { Stars_on = true; emit ConsoleOutput("Sky: Stars are on"); return; }
        if (value == "off") { Stars_on = false; emit ConsoleOutput("Sky: Stars are off"); return; }
    }

    for (auto item: CelestialPack) {
        if (var == item->getID() + ".date") { item->setDate(Angle(qreal(value.toDouble()))); emit ConsoleOutput("Sky: Date of " + item->getID() + " is changed: " + QString::number(item->getDate())); return; }
        if (var == item->getID() + ".time") { item->setTime(Angle(qreal(value.toDouble()))); emit ConsoleOutput("Sky: Time of " + item->getID() + " is changed: " + QString::number(item->getTime())); return; }
    }
    Save();
    emit ConsoleOutput("Sky: Unknown variable: " + var);
}
