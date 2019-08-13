#ifndef GLOBAL_H
#define GLOBAL_H

#include <QString>
#include <QVector3D>

struct CelestialInfo {

    QString     name;
    QVector3D   vect1;
    float       distance;
    qreal       angular_size;
    qreal       phase;
};

struct StarInfo {

    QVector3D   coord;
    quint8      type;
};

#endif // GLOBAL_H
