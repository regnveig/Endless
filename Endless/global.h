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

struct celestial_data {

    QString     name;
    QVector3D   vect1;
    float       distance;
    qreal       angular_size;
};

struct star {

    QVector3D   coord;
    quint8      type;
};

struct GlobalFunc {

    static QVector3D        MatrixRotate        (QVector3D vect, qreal angle_x, qreal angle_y, qreal angle_z);
    static QList<QVector3D> SphericRandom       (quint32 num, quint32 Seed);
};

#endif // GLOBAL_H
