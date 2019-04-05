#ifndef SKY_H
#define SKY_H

#include "global.h"

class Sky : public QObject {

    Q_OBJECT

public:

    explicit        Sky             (QObject *parent = nullptr);
                    ~Sky            () override;

signals:

    void            Data            (QList<celestial_data>);

public slots:

    void            Play            ();
    void            Pause           ();

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
        void                setPlace            (Celestial *new_ground, qreal new_latitude, qreal new_longitude);
        c_system            System              ();

    private:

        QVector3D           AxisRotate          (QVector3D vect, qreal angle);
        Celestial           *ground;
        qreal               *latitude           = new qreal(),
                            *longitude          = new qreal();
    };

    static qreal    Angle           (qreal angle);
    void            Loop            ();
    int             *Timer_ID       = new int();
    Celestial       *Sun            = new Celestial(nullptr, QString("sun"),
                                                    {0.0f, 100.0f, 0.0, 0.0, 0.0, 0.0}, 0.0, 0.0);
    Celestial       *Earth          = new Celestial(Sun, QString("earth"),
                                                    {25000.0f, 1.0f, 0.0, 3.55e-06, 0.4, 9.6e-05}, 3.71, -1.0);
    Celestial       *Moon           = new Celestial(Earth, QString("moon"),
                                                    {64.0f, 0.3f, 0.5, 2.48e-05, -0.4, 2.48e-05}, 0.0, 0.0);
    Spectator       *Player         = new Spectator(Earth, 0.3, 0.0);
};

class Getter : public QObject {

    Q_OBJECT

public:

    explicit Getter(QObject *parent = nullptr) { }
    virtual ~Getter() { }

public slots:

    void GetData(QList<celestial_data> data) {

        for (auto item = 0; item < data.size(); item++) {
            qDebug() << "\nname: " << qPrintable(data.at(item).name) <<
                        "\nx1: " << qPrintable(QString::number(qreal(data.at(item).vect1.x()))) <<
                        "\ny1: " << qPrintable(QString::number(qreal(data.at(item).vect1.y()))) <<
                        "\nz1: " << qPrintable(QString::number(qreal(data.at(item).vect1.z()))) <<
                        "\nS0: " << qPrintable(QString::number(qreal(data.at(item).distance))) <<
                        "\nas: " << qPrintable(QString::number(data.at(item).angular_size));
        }
    }
};

#endif // SKY_H
