#include "global.h"

QVector3D GlobalFunc::MatrixRotate (QVector3D vect, qreal angle_x, qreal angle_y, qreal angle_z) {

    // Это древняя магия под названием "матрица поворота". Она просто работает, и неважно, как

    if (angle_x != 0.0)
        vect = QVector3D(vect.x(),
                         vect.y() * float(qCos(angle_x)) - vect.z() * float(qSin(angle_x)),
                         vect.y() * float(qSin(angle_x)) + vect.z() * float(qCos(angle_x)));

    if (angle_y != 0.0)
        vect = QVector3D(vect.x() * float(qCos(angle_y)) + vect.z() * float(qSin(angle_y)),
                         vect.y(),
                        -vect.x() * float(qSin(angle_y)) + vect.z() * float(qCos(angle_y)));

    if (angle_z != 0.0)
        vect = QVector3D(vect.x() * float(qCos(angle_z)) - vect.y() * float(qSin(angle_z)),
                         vect.x() * float(qSin(angle_z)) + vect.y() * float(qCos(angle_z)),
                         vect.z());
    return vect;
}

QList<QVector3D> GlobalFunc::SphericRandom(quint32 num, quint32 Seed) {

    // Алгоритм взят отсюда: https://www.fundamental-research.ru/ru/article/view?id=31243.
    // Огромное спасибо авторам.

    QList<QVector3D> list;

    QRandomGenerator general(Seed);

    quint32 item = 0;

    while (item < num) {
        qreal azimuth = qreal(general.generate()) / QRandomGenerator::max() * 2 * M_PI;
        qreal zenith = qreal(general.generate()) / QRandomGenerator::max() * M_PI;
        if ((qreal(general.generate()) / QRandomGenerator::max()) < qSin(zenith)) {
            list.append(QVector3D(float(qSin(zenith) * qCos(azimuth)),
                                  float(qSin(zenith) * qSin(azimuth)),
                                  float(qCos(zenith))));
            item++;
        }
    }

    return list;
}
