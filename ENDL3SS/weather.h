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
constexpr qreal SPEED_LOWERING = 10;
const quint8 MATRIX_SIDE = 3;
constexpr qreal POWER_LOWERING = 1;
const float WIND_LOWERING = 1;
constexpr qreal MASS_LOWERING = 1;

struct MatrixCell { qreal power; QVector3D wind; };

class Cyclone {
public:
    explicit Cyclone(quint64 new_lifetime, QVector3D new_place, bool new_isCyclone);
    ~Cyclone();

    void Loop(QVector3D force_summ);
    bool exists();

    MatrixCell getMatrixCellByPoint(QVector3D spectator);
    qreal getPower();
    QVector3D getPlace();

private:
    quint64 lifetime, time_passed = 0;
    QVector3D place, speed = QVector3D(0.0f, 0.0f, 0.0f);
    bool isCyclone;
};

class Weather : public QObject {

    Q_OBJECT

public:

    explicit Weather(QFileInfo saved_file, int TimerInterval, int SaverInterval, QObject *parent = nullptr);
    ~Weather() override;

signals:

    void WeatherData();
    void ConsoleOutput(QString message);

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
    QList<Cyclone *> CyclonePack;
    QString ground;
    qreal season;
    qreal longitude, latitude;

    int Timer_ID, Saver_ID, CycloneTimer_ID;
    int Timer_interval, Saver_interval, CycloneTimer_Interval;

    MatrixCell Matrix[MATRIX_SIDE][MATRIX_SIDE];

    QSqlDatabase saved_db = QSqlDatabase::addDatabase("QSQLITE");
    QRandomGenerator rand;

};

#endif // WEATHER_H
