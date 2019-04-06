#ifndef GUI_OPENGL_H
#define GUI_OPENGL_H

#include "global.h"

class GUI_OpenGL : public QOpenGLWidget {

    Q_OBJECT

public:

    GUI_OpenGL(QWidget *parent = nullptr);

public slots:

    void SkyData(QList<celestial_data> data);

protected:

   void initializeGL() override;
   void resizeGL(int nWidth, int nHeight) override;
   void paintGL() override;

   void mousePressEvent(QMouseEvent* event) override;
   void mouseMoveEvent(QMouseEvent* event) override;

private:

    struct Sky {

        static void PlaceSkylight(QVector3D *vect);
        static void SkyBox();
        static void DoSun(QVector3D vect, GLfloat Radius);
        static void DoMoon(QVector3D vect, GLfloat Radius, qreal Phase, qreal SunDirection);

        static void DoStar(QVector3D vect, GLfloat Radius);
        static void DoStar(QVector3D vect, GLfloat Radius, GLfloat Inner_Radius, quint8 Ray_num);

        static void DoSunCorona(QVector3D vect, GLfloat Radius);
    };

    QPoint pressPosition;
    QPoint releasePosition;

    GLfloat xAxisRotation;
    GLfloat yAxisRotation;
    GLfloat currentWidth;
    GLfloat currentHeight;

    QList<celestial_data> sky_data;
};

#endif // GUI_OPENGL_H
