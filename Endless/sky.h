#ifndef SKY_H
#define SKY_H

#include "global.h"

class Sky : public QObject {

    Q_OBJECT

public:

    explicit        Sky             (QObject *parent = nullptr);
                    ~Sky            () override;

signals:

    void            CelestialData   (QList<celestial_data>);
    void            StarsData       (QList<star>);
    void            GroundData      (ground_data);

public slots:

    void            Play            (bool play);

protected:

    void            timerEvent      (QTimerEvent *event) override;

private:

    struct celestial_const {

        float               distance,           // Расстояние до родителя (Радиус Земли принят за 1)
                            radius;             // Радиус тела (Радиус Земли принят за 1)
        qreal               angle_ecliptic,     // Угол к плоскости эклиптики родителя, рад
                            angle_speed,        // Угловая скорость, рад/цикл
                            angle_axis,         // Угол оси (по отношению к нормали эклиптики), рад
                            rotation_speed;     // Скорость вращения, рад/цикл
    };

    struct c_system {

        QVector3D           axis_x,
                            axis_y,
                            axis_z;
    };

    class Celestial {
    public:

        explicit            Celestial           (Celestial *parent, QString new_name, celestial_const new_const,
                                                 qreal new_angle, qreal new_time);
                            ~Celestial          ();

        int                 AddChild            (Celestial *child);
        celestial_const     getCelestialConst   ();
        QList<Celestial *>  getFamily           ();
        QString             getName             ();
        QVector3D           getPosition         ();
        qreal               getTime             ();
        qreal               getAngle            () { return *angle; }
        void                LoopFamily          ();

    private:

        Celestial           *parent;
        QList<Celestial *>  *children           = new QList<Celestial *>();
        QString             *name               = new QString();
        celestial_const     *c_const            = new celestial_const();
        qreal               *angle              = new qreal(),
                            *time               = new qreal();
    };

    class Spectator {
    public:

        explicit            Spectator           (Celestial *new_ground, qreal new_latitude, qreal new_longitude);
                            ~Spectator          ();
        Celestial *         getGround           ();
        qreal               getLatitude         () { return *latitude; }
        qreal               getLongitude        () { return *longitude; }
        void                setPlace            (Celestial *new_ground, qreal new_latitude, qreal new_longitude);
        c_system            System              ();

    private:

        Celestial           *ground;
        qreal               *latitude           = new qreal(),
                            *longitude          = new qreal();
    };

    static qreal    Angle           (qreal angle);
    static bool     CelestialSort   (const celestial_data &a, const celestial_data &b);
    void            SendCelestial   ();
    void            SendGround      ();
    void            SendStars       ();
    int             *Timer_ID       = new int();
    Celestial       *Sun            = new Celestial(nullptr, QString("sun"),
                                                    {0.0f, 100.0f, 0.0, 0.0, 0.0, 0.0}, 0.0, 0.0);
    Celestial       *Earth          = new Celestial(Sun, QString("earth"),
                                                    {2500.0f, 1.0f, 0.0, 3.55e-06, 0.4, 9.6e-05}, 3.71, -1.0);
    Celestial       *Moon           = new Celestial(Earth, QString("moon"),
                                                    {10.0f, 0.3f, 0.5, 2.48e-05, -0.4, 2.48e-05}, 4.1, 0.0);
    Spectator       *Player         = new Spectator(Earth, 0.3, 0.0);
    QList<star>     *StarPack       = new QList<star>();
};

#endif // SKY_H
