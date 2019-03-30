#ifndef WORLD_H
#define WORLD_H

#include <QObject>
#include <QRunnable>
#include <QTimerEvent>
#include <QtMath>
#include <limits>

const auto  SUNTIME_INTERVAL    = 20u,
            SUNDAY_INTERVAL     = 1000u,
            MOONTIME_INTERVAL   = 10u,
            MOONDAY_INTERVAL    = 50u,
            WEATHER_INTERVAL    = 1000u, // sec
            SUN_DEVIATION_COE   = 4096u,
            SUN_AMPLITUDE       = 2200u,
            MOON_DEVIATION_COE  = 0u,
            MOON_AMPLITUDE      = 5500u;

enum class weather_type {

    Clear,
    Cloudy,
    Rain,
    Storm,
    Snow,
    Blizzard

};

struct polar {

    quint16         azimuth,
                    zenith;
};

struct weather {

    weather_type    type;
    int             id;
    quint16         time_sec;
};

struct skylight {

    int             time_id,
                    day_id;
    quint16         time,
                    day;
};

class World : public QObject {

    Q_OBJECT

public:

    explicit World(QObject *parent, skylight newSun, skylight newMoon, weather newWeather);
    ~World() override;

signals:

    void NewWeather(weather_type);

public slots:

    void Play();
    void Pause();

private:

    skylight    *Sun,
                *Moon;
    weather     *Weather;

    polar toPolar(skylight *SkyLight, quint16 Deviation, quint16 Amplitude);

protected:

    void timerEvent(QTimerEvent *event) override;

};

#endif // WORLD_H
