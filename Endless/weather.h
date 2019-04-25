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

        explicit            Cyclone             (quint32 new_lifetime, qreal new_power, QVector3D new_vect);
                            ~Cyclone            ();
        void                Loop                (QVector3D force_summ);
        bool                Is                  ();
        qreal               getPower            ();
        QVector3D           getVect             ();

    private:

        quint32             xtime = 0,
                            lifetime;
        qreal               power;
        QVector3D           vect;
    };

    void Loop();

    QList<Cyclone>  *CyclonePack    = new QList<Cyclone>();

    int             *Timer_ID       = new int();
    qreal           *season          = new qreal(0.0),
                    *longitude       = new qreal(0.0),
                    *latitude        = new qreal(0.0);
};

#endif // WEATHER_H
