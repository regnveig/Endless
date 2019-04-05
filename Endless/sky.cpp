#include "sky.h"

Celestial::Celestial(Celestial *new_parent, QString new_name, celestial_const new_const, qreal new_angle) : parent(new_parent) {

    *name = new_name; // проверка на уникальность?
    *c_const = new_const;
    *angle = Angle(new_angle);
    if (parent != nullptr) parent->AddChild(this);
}

Celestial::~Celestial() {

    while(!children->isEmpty())
        delete children->takeFirst();
    delete children; children = nullptr;
    parent = nullptr;
    delete name; name = nullptr;
    delete c_const; c_const = nullptr;
    delete angle; angle = nullptr;
};

int Celestial::AddChild(Celestial *child) {
    for (auto item = 0; item < children->size(); item++)
        if (children->at(item) == child) return 1;
    children->append(child);
    return 0;
}

cartesian Celestial::getPosition() {
    if (parent == nullptr)
        return {0.0, 0.0, 0.0};
    qreal x = qCos(*angle) * qCos(c_const->angle_ecliptic) * c_const->radius;
    qreal y = qSin(*angle) * qCos(c_const->angle_ecliptic) * c_const->radius;
    qreal z = qSin(*angle) * qSin(c_const->angle_ecliptic) * c_const->radius;
    cartesian ppos = parent->getPosition();
    return {ppos.x + x, ppos.y + y, ppos.z + z};
}

QString Celestial::getName() {

    return *name;
}

qreal Celestial::getTime() {

    return *time;
}

QList<Celestial *> Celestial::getFamily() {
    QList<Celestial *> family = *children;
    for (auto item = 0; item < children->size(); item++)
        family.append(children->at(item)->getFamily());
    return family;
}

celestial_const Celestial::getCelestialConst() {

    return *c_const;
}

void Celestial::LoopFamily() {

    for (auto item = 0; item < children->size(); item++)
        children->at(item)->LoopFamily();

    if (parent == nullptr) return;

    *angle = Angle(*angle + c_const->angle_speed);
    *time = Angle(*time + c_const->rotation_speed);
}

qreal Celestial::Angle(qreal angle) {

    if (angle >= (2 * M_PI))
        return (angle - (qFloor(angle / (2 * M_PI)) * 2 * M_PI));
    if (angle < 0)
        return (angle + (qFloor(qFabs(angle / (2 * M_PI))) * 2 * M_PI));
    return angle;
}

// -------------------------------------------

Spectator::Spectator(Celestial *new_ground, qreal new_latitude, qreal new_longitude) : ground(new_ground) {

    // CHECK

    *latitude = new_latitude;
    *longitude = new_longitude;
}

Spectator::~Spectator() {

    delete latitude; latitude = nullptr;
    delete longitude; longitude = nullptr;
    ground = nullptr;
}

void Spectator::setLatitude(qreal new_latitude) {

    // CHECK

    *latitude = new_latitude;
}

void Spectator::setLongitude(qreal new_longitude) {

    // CHECK

    *longitude = new_longitude;
}

void Spectator::System(c_system *system) {

    qreal al = Celestial::Angle(*longitude + ground->getTime());
    celestial_const c_const = ground->getCelestialConst();

    cartesian z = {qCos(*latitude) * qCos(al) * c_const.radius,
                   qCos(*latitude) * qSin(al) * c_const.radius,
                   qSin(*latitude) * c_const.radius};

    cartesian x = {qCos(*latitude - M_PI_4) * qCos(al) * c_const.radius,
                   qCos(*latitude - M_PI_4) * qSin(al) * c_const.radius,
                   qSin(*latitude - M_PI_4) * c_const.radius};

    cartesian y = {qCos(al + M_PI_4) * c_const.radius,
                   qSin(al + M_PI_4) * c_const.radius,
                   0};

    system->axis_x = {x.x * qCos(c_const.angle_axis) + x.z * qSin(c_const.angle_axis),
                      x.y,
                      - x.x * qSin(c_const.angle_axis) + x.z * qCos(c_const.angle_axis)};

    system->axis_y = {y.x * qCos(c_const.angle_axis) + y.z * qSin(c_const.angle_axis),
                      y.y,
                      - y.x * qSin(c_const.angle_axis) + y.z * qCos(c_const.angle_axis)};

    system->axis_z = {z.x * qCos(c_const.angle_axis) + z.z * qSin(c_const.angle_axis),
                      z.y,
                      - z.x * qSin(c_const.angle_axis) + z.z * qCos(c_const.angle_axis)};
}

// -------------------------------------------

Sky::Sky(QObject *parent) : QObject(parent) {

}

Sky::~Sky() {

}

void Sky::Loop() {

}
