#ifndef WEATHER_H
#define WEATHER_H

#include "global.h"

class Weather : public QObject {

    Q_OBJECT

public:

    explicit Weather(QObject *parent = nullptr);
    ~Weather() override;

signals:

    void WeatherData(QList<weather_data>);

public slots:

    void GroundData(ground_data);
    void Play(bool play);

protected:

    void timerEvent(QTimerEvent *event) override;

private:

    class Cyclone {
    public:

                            Cyclone             (quint32 new_lifetime, qreal new_power, QVector3D new_vect);
                            ~Cyclone            ();
        void                Loop                (QVector3D force_summ);
        bool                Is                  ();
        qreal               getPower            ();
        QVector3D           getVect             ();

        static const quint32       MIN_LIFETIME        = 128;
        static const quint32       MAX_LIFETIME        = 1024;
        static constexpr qreal     RANDOM_COE          = 0.1;

    private:

        quint32             xtime = 0,
                            lifetime;
        qreal               power;
        QVector3D           vect,
                            speed = QVector3D(0.0f, 0.0f, 0.0f);
    };

    void Loop();
    bool newCyclone(Weather::Cyclone *cyclone, QRandomGenerator *rand);

    QList<Cyclone>  *CyclonePack    = new QList<Cyclone>();

    int             *Timer_ID       = new int();
    qreal           *season         = new qreal(0.0),
                    *longitude      = new qreal(0.0),
                    *latitude       = new qreal(0.0);
};

#endif // WEATHER_H
