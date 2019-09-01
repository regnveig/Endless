#ifndef GUI_OPENGL_H
#define GUI_OPENGL_H

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

#include "global.h"

class GUI_OpenGL : public QOpenGLWidget {

    Q_OBJECT

public:

    GUI_OpenGL                  (QWidget *parent = nullptr);

public slots:

    void                WeatherData                 (QList<CloudInfo> clouds);
    void                CelestialData               (QList<CelestialInfo> data);
    void                StarsData                   (QList<StarInfo> data);
    // -------- Debug ----------
    void                WeatherData                 (QList<weather_data> data);
    // -------------------------
protected:

    void                 initializeGL                () override;
    void                 resizeGL                    (int nWidth, int nHeight) override;
    void                 paintGL                     () override;
    void                 mousePressEvent             (QMouseEvent* event) override;
    void                 mouseMoveEvent              (QMouseEvent* event) override;

private:

    struct Weather {

        static void         DoWeatherBox            ();
        static void         DoCloud                 (CloudInfo cloud);

        static constexpr GLfloat CLOUD_SIZE              = 0.5f;
        static constexpr GLfloat WEATHER_SIZE            = 25.0f;
        static constexpr GLfloat WEATHER_HEIGHT          = 5.0f;
    };

    struct Sky {

        static void         DoMoon                  (QVector3D vect, GLfloat Radius, GLfloat Z,
                                                        qreal Phase, qreal SunDirection);
        static void         DoSkyBox                (GLfloat Z);
        static void         DoStar                  (QVector3D vect, GLfloat Radius);
        static void         DoStar                  (QVector3D vect, GLfloat Radius, GLfloat Inner_Radius, quint8 Ray_num);
        static void         DoSun                   (QVector3D vect, GLfloat Radius);
        static void         DoSunCorona             (QVector3D vect, GLfloat Radius, GLfloat Z);
        static void         PlaceSkylight           (QVector3D *vect);

        static constexpr GLfloat SKY_SIZE                = 5.0f;
        static constexpr GLfloat SKY_BOTTOM              = -0.07f;
    };

    void                    DrawSky();
    void                    DrawWeather();

    QList<CelestialInfo>    sky_data;
    QList<StarInfo>         stars_data;
    QList<CloudInfo>        clouds_data;

    GLfloat                 currentWidth,
                            currentHeight;
    QPoint                  pressPosition;
    QPoint                  releasePosition;
    GLfloat                 xAxisRotation,
                            yAxisRotation;
    // debug
    QList<weather_data>     cyclone_data;
    // ------
};

#endif // GUI_OPENGL_H
