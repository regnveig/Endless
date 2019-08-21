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

class Sky : public QObject {

    Q_OBJECT

public:

    explicit Sky(QFileInfo saved_file, QObject *parent = nullptr);
    ~Sky() override;

    static qreal Angle(qreal angle);

signals:

    void CelestialData(QList<CelestialInfo>);
    void StarsData(QList<StarInfo>);
    void ConsoleOutput(QString message);

public slots:

    void Play(bool play);
    void Save();
    void Console(QString var, QString value);

protected:

    void timerEvent (QTimerEvent *event) override;

private:

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

        void setDate(qreal new_date) { date = Sky::Angle(new_date); }
        void setTime(qreal new_time) { time = Sky::Angle(new_time); }

    private:

        Celestial * parent;
        QList<Celestial *> children;

        QString id;
        float distance, radius;
        qreal ecliptic, axis;
        qreal speed, rotation;
        qreal date, time;

    };

    struct System3D { QVector3D axis_x, axis_y, axis_z; };

    System3D System();
    void SendStars();
    void SendCelestial();

    QVector3D MatrixRotate(QVector3D vect, qreal angle_x, qreal angle_y, qreal angle_z);
    QVector3D SphericRandom(QRandomGenerator *rand);
    static bool CelestialSort(const CelestialInfo &a, const CelestialInfo &b);

    QList<Celestial *> CelestialPack;
    QList<StarInfo> StarPack;
    bool Stars_on = true;

    Celestial * ground;
    qreal longitude, latitude;

    int Timer_ID, Saver_ID;
    int Timer_interval, Saver_interval;

    QSqlDatabase saved_db = QSqlDatabase::addDatabase("QSQLITE");

};

#endif // SKY_H
