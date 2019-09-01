#ifndef WEATHER_H
#define WEATHER_H

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
#include <QMutableListIterator>

#include "global.h"

const quint32 MIN_LIFETIME = 128;
const quint32 MAX_LIFETIME = 1024;
constexpr qreal RANDOM_COE = 0.1;
const float SPEED_LOWERING = 0.01f;
const quint8 MATRIX_SIDE = 5;
constexpr qreal POWER_LOWERING = 1000;
const float WIND_LOWERING = 1.0f;
constexpr qreal SURFACE_COE = 0.01;
const float CELL_SIZE = 0.2f;
constexpr qreal CLOUD_START = 0.25;
const quint16 CLOUD_CYCLES = 100;
constexpr qreal CLOUDS_NUM_COE = 10;

struct MatrixCell { qreal power; QVector3D wind, place; quint16 clouds = 0; };

struct Cloud { qreal x, y; quint8 z; quint16 cycles = CLOUD_CYCLES; };

class Cyclone {
public:
    explicit Cyclone(quint64 new_lifetime, quint64 new_time_passed, QVector3D new_place, QVector3D new_speed, bool new_isCyclone);

    void Loop(QVector3D force_summ);
    bool exists();

    MatrixCell getMatrixCellByPoint(QVector3D spectator);
    qreal getPower();
    QVector3D getPlace();

    quint64 getLifetime() { return lifetime; }
    quint64 getTimePassed() { return time_passed; }
    QVector3D getSpeed() { return speed; }
    bool getIsCyclone() { return isCyclone; }

private:
    quint64 lifetime, time_passed;
    QVector3D place, speed;
    bool isCyclone;
};

class Weather : public QObject {

    Q_OBJECT

public:

    explicit Weather(QFileInfo saved_file, QObject *parent = nullptr);
    ~Weather() override;

signals:

    void WeatherData(QList<CloudInfo> clouds);
    void ConsoleOutput(QString message);
    // -------- Debug ----------
    void                WeatherData                 (QList<weather_data> data);
    // -------------------------

public slots:

    void Play(bool play);
    void Save();
    void Console(QString var, QString value);
    void CelestialData(QList<CelestialInfo> data);

protected:

    void timerEvent (QTimerEvent *event) override;

private:

    bool newCyclone(Cyclone *cyclone);
    void LoopCyclone();
    void LoopMatrix();
    void PlaceMatrix();
    QList<Cyclone *> CyclonePack;
    QList<Cloud *> CloudPack;
    QString ground;
    qreal season;
    qreal longitude, latitude;

    int Timer_ID, Saver_ID, CycloneTimer_ID;
    int Timer_interval, Saver_interval, CycloneTimer_interval;

    MatrixCell Matrix[MATRIX_SIDE][MATRIX_SIDE];

    const QString ConnectionName = QLatin1String("WeatherConnection");
    QSqlDatabase saved_db = QSqlDatabase::addDatabase("QSQLITE", ConnectionName);
    QRandomGenerator rand;
    QRandomGenerator clouder;

};

#endif // WEATHER_H
