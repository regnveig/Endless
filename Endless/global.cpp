#include "global.h"

QVector3D GlobalFunc::MatrixRotate(QVector3D vect, qreal angle_x, qreal angle_y, qreal angle_z) {

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

QVector3D GlobalFunc::SphericRandom(QRandomGenerator *rand) {

    bool    trig        = true;
    qreal   azimuth     = 0.0,
            zenith      = 0.0,
            f           = 0.0;

    while (trig) {

        azimuth = qreal(rand->generate()) / QRandomGenerator::max() * 2 * M_PI;
        zenith  = qreal(rand->generate()) / QRandomGenerator::max() * M_PI;
        f       = qreal(rand->generate()) / QRandomGenerator::max();

        if (f < qSin(zenith))
            trig = false;
    }

    return QVector3D(float(qSin(zenith) * qCos(azimuth)),
                     float(qSin(zenith) * qSin(azimuth)),
                     float(qCos(zenith)));
}
