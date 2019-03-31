#ifndef GUI_OPENGL_H
#define GUI_OPENGL_H

#include <QtGui>
#include <QWidget>
#include <QOpenGLWidget>

class GUI_OpenGL : public QOpenGLWidget {
public:

    GUI_OpenGL(QWidget *parent = nullptr);

protected:

   void initializeGL() override;
   void resizeGL(int nWidth, int nHeight) override;
   void paintGL() override;

private:

   void DoSun(GLfloat sky_color[], GLfloat X, GLfloat Y, GLfloat Radius);
};

#endif // GUI_OPENGL_H
