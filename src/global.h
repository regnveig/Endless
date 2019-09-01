#ifndef GLOBAL_H
#define GLOBAL_H

#include <QString>
#include <QVector3D>

// ------- Debug -------
struct weather_data {

    QVector3D   vect;
    qreal       power;
};
// ---------------------

struct CelestialInfo {

    QString     name;
    QVector3D   vect1;
    float       distance;
    qreal       angular_size;
    qreal       phase;
    qreal       season;
};

struct StarInfo {

    QVector3D   coord;
    quint8      type;
};

struct CloudInfo { qreal x, y; quint8 z; };

#endif // GLOBAL_H
