#ifndef WORLD_H
#define WORLD_H

#include <QObject>
#include <QTimerEvent>
#include <QtMath>
#include <limits>

const auto  SUNTIME_INTERVAL    = 20u,      // сутки длятся 22 минуты.
            SUNDAY_INTERVAL     = 540u,     // год длится 9,8 часов, сезон длится около 2,5 часов (~7 игровых дней).
            MOONTIME_INTERVAL   = 10u,
            MOONDAY_INTERVAL    = 50u,
            SUN_DEVIATION_COE   = 4096u,
            SUN_AMPLITUDE       = 2200u,
            MOON_DEVIATION_COE  = 0u,
            MOON_AMPLITUDE      = 5500u,
            SUNMOON_COE         = 500u;     // Отношение расстояний до луны и солнца. Нужно для правильного определения фазы

const quint16 QUINT16_MAX       = std::numeric_limits<quint16>::max();

const auto SUNKEN_COE = 0.3;

enum class weather_type {

    Clear,
    Cloudy,
    Rain,
    Storm,
    Snow,
    Blizzard
};

struct polar { qreal azimuth, zenith; };

struct cartesius { qreal x, y, z; };

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

    void NewWeather(weather);
    void NewSun(polar);
    void NewMoon(polar);

public slots:

    void Play();
    void Pause();

private:

    skylight    *Sun,
                *Moon;
    weather     *Weather;

    qreal QUIntToRad(quint16 num) { return (num * 2 * M_PI / QUINT16_MAX); }

    polar PolarSun();
    polar PolarMoon();
    qreal LunarPhase();

protected:

    void timerEvent(QTimerEvent *event) override;

};

#endif // WORLD_H
