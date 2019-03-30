#ifndef WORLD_H
#define WORLD_H

#include <QObject>
#include <QRunnable>
#include <QTimerEvent>

const auto  SUNTIME_INTERVAL    = 20u,
            SUNDAY_INTERVAL     = 1000u,
            MOONTIME_INTERVAL   = 10u,
            MOONDAY_INTERVAL    = 50u,
            WEATHER_INTERVAL    = 1000u; // sec

enum class weather_type {

    Clear,
    Cloudy,
    Rain,
    Storm,
    Snow,
    Blizzard

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

    explicit World(QObject *parent);
    ~World() override;

signals:

    void NewSunTime(quint16);
    void NewMoonTime(quint16);
    void NewSunDay(quint16);
    void NewMoonDay(quint16);
    void NewWeather(weather_type);

public slots:

    void Play();
    void Pause();

private:

    skylight    *Sun,
                *Moon;
    weather     *Weather;

protected:

    void timerEvent(QTimerEvent *event) override;

};

#endif // WORLD_H
