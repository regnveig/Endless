#ifndef GLOBAL_H
#define GLOBAL_H

#include <QApplication>
#include <QDebug>
#include <QObject>
#include <QList>
#include <QTimerEvent>
#include <QtMath>
#include <QThread>
#include <QVector3D>

struct celestial_data {

    QString name;
    QVector3D vect1;
    float distance;
    qreal angular_size;
};

#endif // GLOBAL_H
