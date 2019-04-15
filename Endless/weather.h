#ifndef WEATHER_H
#define WEATHER_H

#include "global.h"

struct weather_data {

    enum class weather_type {

        Clear,
        Fog,
        Rain,
        Storm,
        Cloudy,
        Snow,
        Blizzard
    };

    weather_type    type;
    qreal           wind_angle,
                    wind_strength;
    qreal           weather_angle,
                    weather_strength_start,
                    weather_strength_end;       // Для WeatherBox'а

};

class Cyclone {
public:

                    Cyclone         (qint8 new_strength, QVector3D new_eye_coord, quint32 new_life_time);
                    ~Cyclone();
    bool            Tick            (QVector3D summar_power);
    qint8           getStrength     ();
    QVector3D       getEyeCoord     ();

private:

    qint8           *strength       = new qint8();       // + for cyclone, - for anticyclone
    QVector3D       *eye_coord      = new QVector3D();
    quint32         *life_time      = new quint32();      // sec?
};

class Weather : public QObject {

    Q_OBJECT

public:

    explicit        Weather         (QObject *parent = nullptr);
                    ~Weather        () override;

signals:

    void            Data            ();

public slots:

    void            Play            (bool play);
    void            SkyData         (QList<celestial_data> data);

protected:

    void            timerEvent      (QTimerEvent *event) override;

private:

    float           *Z              = new float();
    int             *Timer_ID       = new int();
    QList<Cyclone>  *cyclones       = new QList<Cyclone>();
};

#endif // WEATHER_H
