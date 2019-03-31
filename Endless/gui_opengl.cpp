#include "gui_opengl.h"

GUI_OpenGL::GUI_OpenGL(QWidget *parent) : QOpenGLWidget(parent) {

    resize(600,600);
}

void GUI_OpenGL::initializeGL() {

   glEnable(GL_DEPTH_TEST);
   glShadeModel(GL_SMOOTH);
   glEnable(GL_CULL_FACE);
   glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
}

void GUI_OpenGL::resizeGL(int nWidth, int nHeight) {

    glViewport(0, 0, nHeight, nHeight);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
}

void GUI_OpenGL::DoSun(GLfloat sky_color[], GLfloat X, GLfloat Y, GLfloat Radius) {

    GLfloat light_color1[] = {0.9f, 0.9f, 0.9f};
    GLfloat light_color[] = {1.0f, 1.0f, 1.0f};
    GLfloat sun_edge_color[] = {0.94f, 0.93f, 0.57f};

    glColor3fv (light_color);
    glBegin(GL_QUADS);
    glVertex2f(Radius * 0.225f + X, Radius * 0.225f + Y);
    glVertex2f(Radius * (-0.225f) + X, Radius * 0.225f + Y);
    glVertex2f(Radius * (-0.225f) + X, Radius * (-0.225f) + Y);
    glVertex2f(Radius * 0.225f + X, Radius * (-0.225f) + Y);
    glEnd();
    glBegin(GL_QUADS);
    glVertex2f(X, Radius * 0.315f + Y);
    glVertex2f(Radius * (-0.315f) + X, Y);
    glVertex2f(X, Radius * (-0.315f) + Y);
    glVertex2f(Radius * 0.315f + X, Y);
    glEnd();

    glColor3fv (sun_edge_color);
    glBegin(GL_QUADS);
    glVertex2f(Radius * 0.25f + X, Radius * 0.25f + Y);
    glVertex2f(Radius * (-0.25f) + X, Radius * 0.25f + Y);
    glVertex2f(Radius * (-0.25f) + X, Radius * (-0.25f) + Y);
    glVertex2f(Radius * 0.25f + X, Radius * (-0.25f) + Y);
    glEnd();
    glBegin(GL_QUADS);
    glVertex2f(X, Radius * 0.35f + Y);
    glVertex2f(Radius * (-0.35f) + X, Y);
    glVertex2f(X, Radius * (-0.35f) + Y);
    glVertex2f(Radius * 0.35f + X, Y);
    glEnd();

    glColor3fv (sky_color);
    glBegin(GL_TRIANGLES);
    glVertex2f(Radius + X, Radius + Y);
    glVertex2f(X, Radius * 1.3f + Y);
    glColor3fv (light_color1);
    glVertex2f(X, Y);
    glEnd();
    glColor3fv (sky_color);
    glBegin(GL_TRIANGLES);
    glVertex2f(X, Radius * 1.3f + Y);
    glVertex2f(-Radius + X, Radius + Y);
    glColor3fv (light_color1);
    glVertex2f(X, Y);
    glEnd();

    glColor3fv (sky_color);
    glBegin(GL_TRIANGLES);
    glVertex2f(-Radius + X, Radius + Y);
    glVertex2f(-(Radius * 1.3f) + X, Y);
    glColor3fv (light_color1);
    glVertex2f(X, Y);
    glEnd();
    glColor3fv (sky_color);
    glBegin(GL_TRIANGLES);
    glVertex2f(-(Radius * 1.3f) + X, Y);
    glVertex2f(-Radius + X, -Radius + Y);
    glColor3fv (light_color1);
    glVertex2f(X, Y);
    glEnd();

    glColor3fv (sky_color);
    glBegin(GL_TRIANGLES);
    glVertex2f(-Radius + X, -Radius + Y);
    glVertex2f(X, -(Radius * 1.3f) + Y);
    glColor3fv (light_color1);
    glVertex2f(X, Y);
    glEnd();
    glColor3fv (sky_color);
    glBegin(GL_TRIANGLES);
    glVertex2f(X, -(Radius * 1.3f) + Y);
    glVertex2f(Radius + X, -Radius + Y);
    glColor3fv (light_color1);
    glVertex2f(X, Y);
    glEnd();

    glColor3fv (sky_color);
    glBegin(GL_TRIANGLES);
    glVertex2f(Radius + X, -Radius + Y);
    glVertex2f(Radius * 1.3f + X, Y);
    glColor3fv (light_color1);
    glVertex2f(X, Y);
    glEnd();
    glColor3fv (sky_color);
    glBegin(GL_TRIANGLES);
    glVertex2f(Radius * 1.3f + X, Y);
    glVertex2f(Radius + X, Radius + Y);
    glColor3fv (light_color1);
    glVertex2f(X, Y);
    glEnd();

}

void GUI_OpenGL::paintGL() {

   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glClearColor(0.4f, 0.62f, 0.79f, 1.0f);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();

   // Солнце.

   GLfloat sky_color[] = {0.4f, 0.62f, 0.79f};
   DoSun(sky_color, 0, 0, 0.6);



}
