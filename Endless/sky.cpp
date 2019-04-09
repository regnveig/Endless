#include "sky.h"

/* ------ CELESTIAL SUBCLASS ------ */

Sky::Celestial::Celestial(Celestial *new_parent, QString new_name, celestial_const new_const,
                          qreal new_angle, qreal new_time) : parent(new_parent) {

    *name = new_name;
    *c_const = new_const;
    *angle = Angle(new_angle);
    *time = Angle(new_time);
    if (parent != nullptr) parent->AddChild(this);
}

Sky::Celestial::~Celestial() {

    while(!children->isEmpty())
        delete children->takeFirst();
    delete children; children = nullptr;
    parent = nullptr;
    delete name; name = nullptr;
    delete c_const; c_const = nullptr;
    delete angle; angle = nullptr;
    delete time; time = nullptr;
};

int Sky::Celestial::AddChild(Celestial *child) {

    // Дополнительная проверка на кольца?

    for (auto item = 0; item < children->size(); item++)
        if (children->at(item) == child) return 1;
    children->append(child);
    return 0;
}

Sky::celestial_const Sky::Celestial::getCelestialConst() { return *c_const; }

QList<Sky::Celestial *> Sky::Celestial::getFamily() {

    QList<Celestial *> family = *children;
    for (auto item = 0; item < children->size(); item++)
        family.append(children->at(item)->getFamily());
    return family;
}

QString Sky::Celestial::getName() { return *name; }

QVector3D Sky::Celestial::getPosition() {

    if (parent == nullptr)
        return {0.0, 0.0, 0.0};
    QVector3D d = QVector3D(float(qCos(*angle) * qCos(c_const->angle_ecliptic)) * c_const->distance,
                   float(qSin(*angle) * qCos(c_const->angle_ecliptic)) * c_const->distance,
                   float(qSin(*angle) * qSin(c_const->angle_ecliptic)) * c_const->distance);

    QVector3D ppos = parent->getPosition();
    return {ppos + d};
}

qreal Sky::Celestial::getTime() { return *time; }

void Sky::Celestial::LoopFamily() {

    for (auto item = 0; item < children->size(); item++)
        children->at(item)->LoopFamily();

    if (parent == nullptr) return;

    *angle = Angle(*angle + c_const->angle_speed);
    *time = Angle(*time + c_const->rotation_speed);
}

/* ------ SPECTATOR SUBCLASS ------ */

Sky::Spectator::Spectator(Celestial *new_ground, qreal new_latitude, qreal new_longitude)
    : ground(new_ground) {

    *latitude = Angle(new_latitude);
    *longitude = Angle(new_longitude);
}

Sky::Spectator::~Spectator() {

    delete latitude; latitude = nullptr;
    delete longitude; longitude = nullptr;
    ground = nullptr;
}

Sky::Celestial * Sky::Spectator::getGround() { return ground; }

void Sky::Spectator::setPlace(Celestial *new_ground, qreal new_latitude, qreal new_longitude) {

    ground = new_ground;
    *latitude = Angle(new_latitude);
    *longitude = Angle(new_longitude);
}

Sky::c_system Sky::Spectator::System() {

    qreal al = Angle(*longitude + ground->getTime());
    celestial_const c_const = ground->getCelestialConst();

    QVector3D z (float(qCos(*latitude) * qCos(al)) * c_const.radius,
                 float(qCos(*latitude) * qSin(al)) * c_const.radius,
                 float(qSin(*latitude)) * c_const.radius);

    QVector3D x (float(qCos(*latitude - M_PI_2) * qCos(al)) * c_const.radius,
                 float(qCos(*latitude - M_PI_2) * qSin(al)) * c_const.radius,
                 float(qSin(*latitude - M_PI_2)) * c_const.radius);

    QVector3D y (float(qCos(al + M_PI_2)) * c_const.radius,
                 float(qSin(al + M_PI_2)) * c_const.radius,
                 0.0f);

    return {GlobalFunc::MatrixRotate(x, 0.0, c_const.angle_axis, 0.0),
            GlobalFunc::MatrixRotate(y, 0.0, c_const.angle_axis, 0.0),
            GlobalFunc::MatrixRotate(z, 0.0, c_const.angle_axis, 0.0)};
}

/* ------ SKY CLASS ------ */

Sky::Sky(QObject *parent) : QObject(parent) {

    quint32 StarNum = 500;
    quint8 StarType = 0;

    QList<QVector3D> list = GlobalFunc::SphericRandom(StarNum, 20);

    while(!list.isEmpty()) {

        if (list.size() < int(StarNum / 10)) StarType = 1;
        if (list.size() < int(StarNum / 50)) StarType = 2;
        StarPack->append({list.takeFirst(), StarType});
    }

    Play(true);
}

Sky::~Sky() {

    Play(false);

    delete Timer_ID; Timer_ID = nullptr;
    delete Player; Player = nullptr;
    delete Sun; Sun = nullptr;
    delete StarPack; StarPack = nullptr;
}

void Sky::Play(bool play) {

    if (play) *Timer_ID = startTimer(1);
    else killTimer(*Timer_ID);
}

void Sky::timerEvent([[maybe_unused]] QTimerEvent *event) { Loop(); }

qreal Sky::Angle(qreal angle) {

    if (angle >= (2 * M_PI))
        return (angle - (qFloor(angle / (2 * M_PI)) * 2 * M_PI));
    if (angle < 0)
        return (angle + (qCeil(qFabs(angle / (2 * M_PI))) * 2 * M_PI));
    return angle;
}

void Sky::Loop() {

    Sun->LoopFamily();

    QList<celestial_data> list;
    c_system System = Player->System();
    QVector3D Pos = (Player->getGround()->getPosition() + System.axis_z);
    QList<Celestial *> Family = Sun->getFamily();
    Family.append(Sun);

    QVector3D SunPos = Sun->getPosition() - Pos;

    for (auto item = 0; item < Family.size(); item++) {

        QString new_name = Family.at(item)->getName();
        QVector3D new_coord = Family.at(item)->getPosition() - Pos;
        float new_distance = new_coord.length();
        qreal new_angular_size =
                2 * qAtan(qreal(Family.at(item)->getCelestialConst().radius / (2.0f * new_distance)));

        QVector3D new_vect1 (QVector3D::dotProduct(new_coord, System.axis_x),
                             QVector3D::dotProduct(new_coord, System.axis_y),
                             QVector3D::dotProduct(new_coord, System.axis_z));
        new_vect1.normalize();

        QVector3D toSpec = QVector3D(0.0f, 0.0f, 0.0f) - new_coord;
        QVector3D toSun = SunPos - new_coord;
        qreal new_phase = 0.0;
        if (!toSun.isNull())
            new_phase = qAcos(qreal(QVector3D::dotProduct(toSpec, toSun) / (toSpec.length() * toSun.length())));

        list.append({new_name, new_vect1, new_distance, new_angular_size, new_phase});
    }

    std::sort(list.begin(), list.end(), CelestialSort);

    emit Data(list);

    QList<star> star_list;

    for (auto item = 0; item < StarPack->size(); item++) {

        QVector3D new_vect (QVector3D::dotProduct(StarPack->at(item).coord, System.axis_x),
                             QVector3D::dotProduct(StarPack->at(item).coord, System.axis_y),
                             QVector3D::dotProduct(StarPack->at(item).coord, System.axis_z));
        new_vect.normalize();
        star_list.append({new_vect, StarPack->at(item).type});
    }

    emit Stars(star_list);
}

bool Sky::CelestialSort(const celestial_data &a, const celestial_data &b) { return a.distance > b.distance; }
