#ifndef GLOBAL_H
#define GLOBAL_H

#include <QtGui>
#include <QtMath>

#include <QApplication>
#include <QDebug>
#include <QList>
#include <QMouseEvent>
#include <QObject>
#include <QOpenGLWidget>
#include <QPoint>
#include <QRandomGenerator>
#include <QThread>
#include <QTimer>
#include <QTimerEvent>
#include <QVector3D>
#include <QWidget>

static QRandomGenerator *GLOBAL_RAND = new QRandomGenerator();

struct celestial_data {

    QString     name;
    QVector3D   vect1;
    float       distance;
    qreal       angular_size;
    qreal       phase;
};

struct star {

    QVector3D   coord;
    quint8      type;
};

struct weather_data {

    QVector3D   vect;
    qreal       power;
};

struct ground_data {

    qreal       season,
                latitude,
                longitude;
};

struct GlobalFunc {

    static QVector3D        MatrixRotate        (QVector3D vect, qreal angle_x, qreal angle_y, qreal angle_z);
    static QVector3D        SphericRandom       (QRandomGenerator *rand);
};

#endif // GLOBAL_H
