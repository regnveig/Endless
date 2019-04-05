#ifndef SKY_H
#define SKY_H

#include "global.h"

struct celestial_const {

    qreal   distance,          // Distance from parent
            angle_ecliptic,    // Angle from parent's ecliptic
            angle_speed,       // Angle speed in rad per loop
            radius,            // Radius of planet
            angle_axis,        // Axis angle (to parent's ecliptic perpendicular)
            rotation_speed;    // Day-night
};

struct cartesian {

    const cartesian operator+(const cartesian& rhs) const { return {x + rhs.x, y + rhs.y, z + rhs.z}; }
    const cartesian operator-(const cartesian& rhs) const { return {x - rhs.x, y - rhs.y, z - rhs.z}; }
    qreal length() const { return (qSqrt(qPow(x, 2) + qPow(y, 2) + qPow(z, 2))); }
    static qreal scalar(cartesian v1, cartesian v2) { return ((v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z)); }

    qreal x, y, z;
};

struct c_system { cartesian axis_x, axis_y, axis_z; };

class Celestial {
public:

    explicit Celestial(Celestial *parent, QString new_name, celestial_const new_const, qreal new_angle, qreal new_time);
    ~Celestial();
    int AddChild(Celestial *child);
    cartesian getPosition();
    QString getName();
    qreal getTime();
    celestial_const getCelestialConst();
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

class Spectator {
public:

    explicit Spectator(Celestial *new_ground, qreal new_latitude, qreal new_longitude);
    ~Spectator();
    void setPlace(Celestial *new_ground, qreal new_latitude, qreal new_longitude);
    Celestial * getGround() { return ground; }
    c_system System();

private:

    cartesian AxisRotate(cartesian vect, qreal angle);

    Celestial   *ground;
    qreal       *latitude = new qreal(),
                *longitude = new qreal();
};

class Sky : public QObject {

    Q_OBJECT

public:

    explicit Sky(QObject *parent = nullptr);
    ~Sky() override;

signals:

    void Data(QList<celestial_data>);

public slots:

    void Play();
    void Pause();

protected:

    void timerEvent(QTimerEvent *event) override;

private:

    void Loop();

    int * Timer_ID = new int();

    celestial_const * SunConst = new celestial_const({0.0, 0.0, 0.0, 100.0, 0.0, 0.0});
    celestial_const * EarthConst = new celestial_const({25000.0, 0.0, 3.55e-06, 1.0, 0.4, 9.6e-05});
    celestial_const * MoonConst = new celestial_const({64.0, 0.5, 2.48e-05, 0.3, -0.4, 2.48e-05});

    Celestial * Sun = new Celestial(nullptr, QString("sun"), *SunConst, 0.0, 0.0);
    Celestial * Earth = new Celestial(Sun, QString("earth"), *EarthConst, 3.71, -1);
    Celestial * Moon = new Celestial(Earth, QString("moon"), *MoonConst, 0.0, 0.0);

    Spectator * Player = new Spectator(Earth, 0.3, 0.0);
};

class Getter : public QObject {

    Q_OBJECT

public:

    explicit Getter(QObject *parent = nullptr) { }
    virtual ~Getter() {}

public slots:

    void GetData(QList<celestial_data> data) {

        for (auto item = 0; item < data.size(); item++) {
            qDebug() << "\nname: " << qPrintable(data.at(item).name) <<
                        "\nx1: " << qPrintable(QString::number(data.at(item).x1)) <<
                        "\ny1: " << qPrintable(QString::number(data.at(item).y1)) <<
                        "\nz1: " << qPrintable(QString::number(data.at(item).z1)) <<
                        "\nS0: " << qPrintable(QString::number(data.at(item).distance)) <<
                        "\nas: " << qPrintable(QString::number(data.at(item).angular_size));
        }
    }
};

#endif // SKY_H
