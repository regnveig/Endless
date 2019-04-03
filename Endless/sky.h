#ifndef SKY_H
#define SKY_H

#include <QObject>
#include <QDebug>
#include <QList>
#include <QTimerEvent>
#include <QtMath>

struct celestial_data {

    QString name;
};

struct celestial_const {

    qreal   distance,          // Distance from parent
            angle_ecliptic,    // Angle from parent's ecliptic
            angle_speed,       // Angle speed in rad per loop
            radius,            // Radius of planet
            angle_axis,        // Axis angle (to parent's ecliptic perpendicular)
            rotation_speed;    // Day-night
};

struct spheric {

    qreal azimuth,
    zenith;
};

struct cartesian {

    qreal x, y, z;
};

class Celestial {
public:

    Celestial(Celestial *parent, QString new_name, celestial_const new_const, qreal new_angle);
    ~Celestial();
    int AddChild(Celestial *child);
    cartesian getPosition();
    QString getName();
    qreal getTime();
    QList<Celestial *> getFamily();
    void LoopFamily();

    static qreal Angle(qreal angle);

private:

    Celestial           *parent;
    QList<Celestial *>  *children   = new QList<Celestial *>();
    QString             *name       = new QString();
    celestial_const     *c_const    = new celestial_const();

    qreal               *angle      = new qreal();
    qreal               *time       = new qreal();
};

struct spectator {

    Celestial * ground;
    qreal latitude,
          longitude;
};

class Sky : public QObject {

    Q_OBJECT

public:

    explicit Sky(QObject *parent = nullptr);
    ~Sky() override;

signals:

    void Data(celestial_data);

public slots:

    void Loop();

private:

    cartesian SpecVec(spectator * spec);

    celestial_const * SunConst = new celestial_const({0.0, 0.0, 0.0, 100.0, 0.0, 0.0});
    celestial_const * EarthConst = new celestial_const({25000.0, 0.0, 3.55e-06, 1.0, 0.4, 9.6e-05});
    celestial_const * MoonConst = new celestial_const({64.0, 0.5, 2.48e-05, 0.3, -0.4, 2.48e-05});

    Celestial * Sun = new Celestial(nullptr, QString("sun"), *SunConst, 0.0);
    Celestial * Earth = new Celestial(Sun, QString("earth"), *EarthConst, 0.0);
    Celestial * Moon = new Celestial(Earth, QString("moon"), *MoonConst, 0.0);
    spectator * Player = new spectator({Earth, 0.3, 0.0});
};

#endif // SKY_H
