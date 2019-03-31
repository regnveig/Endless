#ifndef GUI_OPENGL_H
#define GUI_OPENGL_H

#include <QtGui>
#include <QWidget>
#include <QPoint>
#include <QOpenGLWidget>
#include <QTimer>
#include <QMouseEvent>

enum class star_size {

    Small,
    Large
};

class GUI_OpenGL : public QOpenGLWidget {
public:

    GUI_OpenGL(QWidget *parent = nullptr);

protected:

   void initializeGL() override;
   void resizeGL(int nWidth, int nHeight) override;
   void paintGL() override;

   void mousePressEvent(QMouseEvent* event) override;
   void mouseMoveEvent(QMouseEvent* event) override;

private:

    struct Sky {

        static void SkyBox();
        static void DoSun(GLfloat X, GLfloat Y, GLfloat Radius);
        static void DoMoon(GLfloat X, GLfloat Y, GLfloat Radius, qreal Phase, qreal SunDirection);

        static void DoStar(GLfloat X, GLfloat Y, GLfloat Radius);
        static void DoStar(GLfloat X, GLfloat Y, GLfloat Radius, GLfloat Inner_Radius, quint8 Ray_num);

        static void DoSunCorona(GLfloat X, GLfloat Y, GLfloat Radius);
    };

    QPoint pressPosition;
    QPoint releasePosition;

    GLfloat xAxisRotation;
    GLfloat yAxisRotation;
    GLfloat currentWidth;
    GLfloat currentHeight;
};

#endif // GUI_OPENGL_H
