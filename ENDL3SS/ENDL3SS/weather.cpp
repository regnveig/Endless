#include "weather.h"

void Cyclone::Loop(QVector3D force_summ) {

    speed += force_summ;
    speed.normalize();
    speed /= SPEED_LOWERING;
    place += speed;
    place.normalize();
    time_passed++;
}

MatrixCell Cyclone::getMatrixCellByPoint(QVector3D spectator) {

    qreal power = qSin(time_passed * M_PI / lifetime) * (isCyclone ? -1 : 1);
    qreal s = qAcos(qreal(QVector3D::dotProduct(spectator, place))) * M_PI;

    qreal point_power = power * qPow(2, - (s * POWER_LOWERING / lifetime));

    QVector3D point_wind = QVector3D::crossProduct(spectator, place);
    point_wind.normalize();
    point_wind *= float(point_power) * WIND_LOWERING;

    return {point_power, point_wind};
}

bool Cyclone::exists() { return (time_passed < lifetime); }

Weather::Weather(QFileInfo saved_file, int TimerInterval, int SaverInterval, QObject *parent) : QObject(parent), Timer_interval(TimerInterval), Saver_interval(SaverInterval) {

    // Open DB

    saved_db.setDatabaseName(saved_file.absoluteFilePath());

    if (!saved_db.open()) {
        emit ConsoleOutput(saved_db.lastError().text());
        throw QException();
    }

    QSqlQuery query;

    Play(true);

}

Weather::~Weather() {
    Play(false);
}

void Weather::Play(bool play) {

    if (play) {
        Timer_ID = startTimer(Timer_interval);
        Saver_ID = startTimer(Saver_interval);
    } else {
        killTimer(Timer_ID);
        killTimer(Saver_ID);
    }
}

void Weather::timerEvent([[maybe_unused]] QTimerEvent *event) {

    if (event->timerId() == Timer_ID) {

    }

    if (event->timerId() == Saver_ID) Save();

}

void Weather::Save() {

    QSqlQuery query;

}

void Weather::Console(QString var, QString value) {

}
