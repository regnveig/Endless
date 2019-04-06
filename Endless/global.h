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

    QString name;
    QVector3D vect1;
    float distance;
    qreal angular_size;
};

struct GlobalFunc {

    static QVector3D        MatrixRotate        (QVector3D vect, qreal angle_x, qreal angle_y, qreal angle_z);
};

#endif // GLOBAL_H
