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
