#ifndef SKY_H
#define SKY_H

#include <QObject>
#include <QtSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QException>
#include <QFileInfo>
#include <QVariant>
#include <QVector3D>
#include <QRandomGenerator>
#include <QTimerEvent>

#include "global.h"

QVector3D MatrixRotate(QVector3D vect, qreal angle_x, qreal angle_y, qreal angle_z);
qreal Angle(qreal angle);
QVector3D SphericRandom(QRandomGenerator *rand);
bool CelestialSort(const CelestialInfo &a, const CelestialInfo &b);

class Celestial {
public:

    explicit Celestial(Celestial * parent,
                       QString new_id, float new_distance, float new_radius,
                       qreal new_ecliptic, qreal new_axis, qreal new_speed,
                       qreal new_rotation, qreal new_date, qreal new_time);
    ~Celestial();
    int AddChild(Celestial *child);
    QList<Celestial *> getFamily();
    QVector3D getPosition();
    void LoopFamily();

    QString getID() { return id; }
    float getDistance() { return distance; }
    float getRadius() { return radius; }
    qreal getEcliptic() { return ecliptic; }
    qreal getAxis() { return axis; }
    qreal getSpeed() { return speed; }
    qreal getRotation() { return rotation; }
    qreal getDate() { return date; }
    qreal getTime() { return time; }

private:

    Celestial * parent;
    QList<Celestial *> children;

    QString id;
    float distance, radius;
    qreal ecliptic, axis;
    qreal speed, rotation;
    qreal date, time;

};

class Sky : public QObject {

    Q_OBJECT

public:

    explicit Sky(QFileInfo saved_file, int TimerInterval, int SaverInterval, QObject *parent = nullptr);
    ~Sky() override;

signals:

    void  CelestialData(QList<CelestialInfo>);
    void  StarsData(QList<StarInfo>);

public slots:

    void Play(bool play);
    void Save();

protected:

    void timerEvent (QTimerEvent *event) override;

private:

    struct System3D { QVector3D axis_x, axis_y, axis_z; };

    System3D System();
    void SendStars();
    void SendCelestial();

    QList<Celestial *> CelestialPack;
    QList<StarInfo> StarPack;

    Celestial * ground;
    qreal longitude, latitude;

    int Timer_ID, Saver_ID;
    int Timer_interval, Saver_interval;

    QSqlDatabase saved_db = QSqlDatabase::addDatabase("QSQLITE");

};

#endif // SKY_H
