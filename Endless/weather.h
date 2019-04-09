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

                    Cyclone();
                    ~Cyclone();

private:

    qint8           strength;       // + for cyclone, - for anticyclone
    QVector3D       eye_coord;
    quint16         life_time;      // sec?
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

    float           Z;
};

#endif // WEATHER_H
