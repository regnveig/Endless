#ifndef GUI_OPENGL_H
#define GUI_OPENGL_H

#include <QtGui>
#include <QWidget>
#include <QGLWidget>

class GUI_OpenGL : public QGLWidget {
public:

    GUI_OpenGL(QWidget *parent = nullptr);

protected:

   void initializeGL();
   void resizeGL(int nWidth, int nHeight);
   void paintGL();
};

#endif // GUI_OPENGL_H
