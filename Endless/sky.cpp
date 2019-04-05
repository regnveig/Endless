#include "sky.h"

Celestial::Celestial(Celestial *new_parent, QString new_name, celestial_const new_const, qreal new_angle, qreal new_time) : parent(new_parent) {

    *name = new_name;
    *c_const = new_const;
    *angle = Angle(new_angle);
    *time = Angle(new_time);
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
    delete time; time = nullptr;
};

int Celestial::AddChild(Celestial *child) {

    // Дополнительная проверка на кольца?

    for (auto item = 0; item < children->size(); item++)
        if (children->at(item) == child) return 1;
    children->append(child);
    return 0;
}

cartesian Celestial::getPosition() {

    if (parent == nullptr)
        return {0.0, 0.0, 0.0};
    cartesian d = {qCos(*angle) * qCos(c_const->angle_ecliptic) * c_const->distance,
                   qSin(*angle) * qCos(c_const->angle_ecliptic) * c_const->distance,
                   qSin(*angle) * qSin(c_const->angle_ecliptic) * c_const->distance};

    cartesian ppos = parent->getPosition();
    return {ppos + d};
}

QString Celestial::getName() { return *name; }

qreal Celestial::getTime() { return *time; }

celestial_const Celestial::getCelestialConst() { return *c_const; }

QList<Celestial *> Celestial::getFamily() {

    QList<Celestial *> family = *children;
    for (auto item = 0; item < children->size(); item++)
        family.append(children->at(item)->getFamily());
    return family;
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
        return (angle + (qCeil(qFabs(angle / (2 * M_PI))) * 2 * M_PI));
    return angle;
}

// -------------------------------------------

Spectator::Spectator(Celestial *new_ground, qreal new_latitude, qreal new_longitude) : ground(new_ground) {

    *latitude = Celestial::Angle(new_latitude);
    *longitude = Celestial::Angle(new_longitude);
}

Spectator::~Spectator() {

    delete latitude; latitude = nullptr;
    delete longitude; longitude = nullptr;
    ground = nullptr;
}

void Spectator::setPlace(Celestial *new_ground, qreal new_latitude, qreal new_longitude) {

    ground = new_ground;
    *latitude = Celestial::Angle(new_latitude);
    *longitude = Celestial::Angle(new_longitude);
}

cartesian Spectator::AxisRotate(cartesian vect, qreal angle) {

    return { vect.x * qCos(angle) + vect.z * qSin(angle),
             vect.y,
            -vect.x * qSin(angle) + vect.z * qCos(angle)};
}

c_system Spectator::System() {

    qreal al = Celestial::Angle(*longitude + ground->getTime());
    celestial_const c_const = ground->getCelestialConst();

    cartesian z = {qCos(*latitude) * qCos(al) * c_const.radius,
                   qCos(*latitude) * qSin(al) * c_const.radius,
                   qSin(*latitude) * c_const.radius};

    cartesian x = {qCos(*latitude - M_PI_2) * qCos(al) * c_const.radius,
                   qCos(*latitude - M_PI_2) * qSin(al) * c_const.radius,
                   qSin(*latitude - M_PI_2) * c_const.radius};

    cartesian y = {qCos(al + M_PI_2) * c_const.radius,
                   qSin(al + M_PI_2) * c_const.radius,
                   0};

    return {AxisRotate(x, c_const.angle_axis),
            AxisRotate(y, c_const.angle_axis),
            AxisRotate(z, c_const.angle_axis)};
}

// -------------------------------------------

Sky::Sky(QObject *parent) : QObject(parent) {

}

Sky::~Sky() {

}

void Sky::Loop() {

    Sun->LoopFamily();

    QList<celestial_data> list;

    c_system System = Player->System();

    cartesian Pos = (Player->getGround()->getPosition() + System.axis_z);
    qreal AxisLength = Player->getGround()->getCelestialConst().radius;

    QList<Celestial *> Family = Sun->getFamily();
    Family.append(Sun);

    for (auto item = 0; item < Family.size(); item++) {

        QString new_name = Family.at(item)->getName();
        cartesian new_coord = Family.at(item)->getPosition() - Pos;
        qreal new_x1 = cartesian::scalar(new_coord, System.axis_x) / (new_coord.length() * AxisLength);
        qreal new_y1 = cartesian::scalar(new_coord, System.axis_y) / (new_coord.length() * AxisLength);
        qreal new_z1 = cartesian::scalar(new_coord, System.axis_z) / (new_coord.length() * AxisLength);
        list.append({new_name, new_x1, new_y1, new_z1});
    }

    emit Data(list);
}
