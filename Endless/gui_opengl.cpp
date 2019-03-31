#include "gui_opengl.h"

GUI_OpenGL::GUI_OpenGL(QWidget *parent) : QOpenGLWidget(parent) {

    resize(600,600);
}

void GUI_OpenGL::initializeGL() {

   glShadeModel(GL_SMOOTH);
   glEnable(GL_CULL_FACE);
   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
   glEnable(GL_BLEND);
   glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

}

void GUI_OpenGL::resizeGL(int nWidth, int nHeight) {

    glViewport(0, 0, nHeight, nHeight);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
}

void GUI_OpenGL::paintGL() {

   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glClearColor(0.0f, 0.02f, 0.0f, 1.0f);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();

   DoMoon(0, 0, 0.2f, 0.0);

}

void GUI_OpenGL::DoSun(GLfloat X, GLfloat Y, GLfloat Radius) {

    //VARIABLE

    GLfloat light_color[]       = {1.0f, 1.0f, 1.0f};
    GLfloat sun_edge_color[]    = {0.94f, 0.93f, 0.57f};

    const GLfloat turn          = GLfloat(qSqrt(0.5));

    GLfloat c4                  = Radius;
    GLfloat c3                  = c4 * turn;
    GLfloat c2                  = Radius * 0.9f;
    GLfloat c1                  = c2 * turn;

    // PRIMITIVE

    glBegin     (GL_QUADS);
    glColor3fv  (sun_edge_color);
    glVertex2f  (X + c3, Y + c3);
    glVertex2f  (X - c3, Y + c3);
    glVertex2f  (X - c3, Y - c3);
    glVertex2f  (X + c3, Y - c3);
    glEnd       ();

    glBegin     (GL_QUADS);
    glColor3fv  (sun_edge_color);
    glVertex2f  (X, Y + c4);
    glVertex2f  (X - c4, Y);
    glVertex2f  (X, Y - c4);
    glVertex2f  (X + c4, Y);
    glEnd       ();

    glBegin     (GL_QUADS);
    glColor3fv  (light_color);
    glVertex2f  (X + c1, Y + c1);
    glVertex2f  (X - c1, Y + c1);
    glVertex2f  (X - c1, Y - c1);
    glVertex2f  (X + c1, Y - c1);
    glEnd       ();

    glBegin     (GL_QUADS);
    glColor3fv  (light_color);
    glVertex2f  (X, Y + c2);
    glVertex2f  (X - c2, Y);
    glVertex2f  (X, Y - c2);
    glVertex2f  (X + c2, Y);
    glEnd       ();
}

void GUI_OpenGL::DoMoon(GLfloat X, GLfloat Y, GLfloat Radius, qreal Phase) {

    // VARIABLE

    GLfloat moon_edge_color[]   = {0.25f, 0.2f, 0.2f};
    GLfloat moon_dot_color[]   = {0.7f, 0.65f, 0.65f};
    GLfloat moon_color[]        = {0.97f, 0.95f, 0.95f};
    GLfloat moon_shadow_color[]   = {0.15f, 0.1f, 0.1f, 0.8f};

    const GLfloat cx            = GLfloat(qSin(M_PI/3));
    const GLfloat cy            = GLfloat(qCos(M_PI/3));
    const GLfloat tri_center    = GLfloat(1 / qSqrt(3));

    GLfloat c1                  = Radius * 0.9f;
    GLfloat c2                  = Radius;
    GLfloat ct                  = c1 * tri_center;
    GLfloat cs                  = Radius * 0.1f;
    GLfloat cm                  = Radius * 0.25f;

    // PRIMITIVE

    glBegin     (GL_POLYGON);
    glColor3fv  (moon_edge_color);
    glVertex2f  (X, Y + c2);
    glVertex2f  (X - c2 * cx, Y + c2 * cy);
    glVertex2f  (X - c2 * cx, Y - c2 * cy);
    glVertex2f  (X, Y - c2);
    glVertex2f  (X + c2 * cx, Y - c2 * cy);
    glVertex2f  (X + c2 * cx, Y + c2 * cy);
    glEnd       ();

    glBegin     (GL_POLYGON);
    glColor3fv  (moon_color);
    glVertex2f  (X, Y + c1);
    glVertex2f  (X - c1 * cx, Y + c1 * cy);
    glVertex2f  (X - c1 * cx, Y - c1 * cy);
    glVertex2f  (X, Y - c1);
    glVertex2f  (X + c1 * cx, Y - c1 * cy);
    glVertex2f  (X + c1 * cx, Y + c1 * cy);
    glEnd       ();

    // dots

    glBegin     (GL_TRIANGLES);
    glColor3fv  (moon_dot_color);
    glVertex2f  (X + ct * cy + cm, Y + ct * cx);
    glVertex2f  (X + (ct - cm) * cy, Y + (ct + cm) * cx);
    glVertex2f  (X + (ct - cm) * cy, Y + (ct - cm) * cx);
    glEnd       ();

    glBegin     (GL_TRIANGLES);
    glColor3fv  (moon_dot_color);
    glVertex2f  (X - ct * (cy + 1), Y + ct * cx);
    glVertex2f  (X, Y);
    glVertex2f  (X, Y + 2 * ct * cx);
    glEnd       ();

    glBegin     (GL_TRIANGLES);
    glColor3fv  (moon_dot_color);
    glVertex2f  (X + ct - cm, Y);
    glVertex2f  (X + ct + cm * cy, Y - cm * cx);
    glVertex2f  (X + ct + cm * cy, Y + cm * cx);
    glEnd       ();

    glBegin     (GL_TRIANGLES);
    glColor3fv  (moon_dot_color);
    glVertex2f  (X - ct + cm, Y);
    glVertex2f  (X - ct - cm * cy, Y + cm * cx);
    glVertex2f  (X - ct - cm * cy, Y - cm * cx);
    glEnd       ();

    glBegin     (GL_TRIANGLES);
    glColor3fv  (moon_dot_color);
    glVertex2f  (X + ct * cy + cs, Y - ct * cx);
    glVertex2f  (X + (ct - cs) * cy, Y - (ct - cs) * cx);
    glVertex2f  (X + (ct - cs) * cy, Y - (ct + cs) * cx);
    glEnd       ();

    glBegin     (GL_TRIANGLES);
    glColor3fv  (moon_edge_color);
    glVertex2f  (X - ct * cy - cm, Y + ct * cx);
    glVertex2f  (X - (ct - cm) * cy, Y + (ct - cm) * cx);
    glVertex2f  (X - (ct - cm) * cy, Y + (ct + cm) * cx);
    glEnd       ();

    // shadow

    glColor4fv (moon_shadow_color);
    glBegin(GL_POLYGON);
    glVertex2f((Radius * 0.68f * cx) + X,
               (Radius * 0.68f * cy) + Y);
    glVertex2f(X, (Radius * 0.8f) + Y);
    glVertex2f(-(Radius * 0.9f * cx) + X,
               (Radius * 0.9f * cy) + Y);
    glVertex2f(-(Radius * 0.9f * cx) + X,
               -(Radius * 0.9f * cy) + Y);
    glVertex2f(X, -(Radius * 0.8f) + Y);
    glVertex2f((Radius * 0.68f * cx) + X,
               -(Radius * 0.68f * cy) + Y);
    glEnd();
}

void GUI_OpenGL::DoSunCorona(GLfloat X, GLfloat Y, GLfloat Radius) {

    // VARIABLE

    GLfloat sky_color[]  = {0.9f, 0.9f, 0.9f, 0.0f};
    GLfloat light_color1[]  = {0.9f, 0.9f, 0.9f, 1.0f};

    const GLfloat turn      = GLfloat(qSqrt(0.5));

    GLfloat c2              = Radius;
    GLfloat c1              = Radius * turn;

    // PRIMITIVE

    glBegin     (GL_TRIANGLES);
    glColor4fv  (sky_color);
    glVertex2f  (X + c1, Y + c1);
    glVertex2f  (X, Y + c2);
    glColor4fv  (light_color1);
    glVertex2f  (X, Y);
    glEnd       ();

    glBegin     (GL_TRIANGLES);
    glColor4fv  (sky_color);
    glVertex2f  (X, Y + c2);
    glVertex2f  (X - c1, Y + c1);
    glColor4fv  (light_color1);
    glVertex2f  (X, Y);
    glEnd       ();

    glBegin     (GL_TRIANGLES);
    glColor4fv  (sky_color);
    glVertex2f  (X - c1, Y + c1);
    glVertex2f  (X - c2, Y);
    glColor4fv  (light_color1);
    glVertex2f  (X, Y);
    glEnd       ();

    glBegin     (GL_TRIANGLES);
    glColor4fv  (sky_color);
    glVertex2f  (X - c2, Y);
    glVertex2f  (X - c1, Y - c1);
    glColor4fv  (light_color1);
    glVertex2f  (X, Y);
    glEnd       ();

    glBegin     (GL_TRIANGLES);
    glColor4fv  (sky_color);
    glVertex2f  (X - c1, Y - c1);
    glVertex2f  (X, Y - c2);
    glColor4fv  (light_color1);
    glVertex2f  (X, Y);
    glEnd       ();

    glBegin     (GL_TRIANGLES);
    glColor4fv  (sky_color);
    glVertex2f  (X, Y - c2);
    glVertex2f  (X + c1, Y - c1);
    glColor4fv  (light_color1);
    glVertex2f  (X, Y);
    glEnd       ();

    glBegin     (GL_TRIANGLES);
    glColor4fv  (sky_color);
    glVertex2f  (X + c1, Y - c1);
    glVertex2f  (X + c2, Y);
    glColor4fv  (light_color1);
    glVertex2f  (X, Y);
    glEnd       ();

    glBegin     (GL_TRIANGLES);
    glColor4fv  (sky_color);
    glVertex2f  (X + c2, Y);
    glVertex2f  (X + c1, Y + c1);
    glColor4fv  (light_color1);
    glVertex2f  (X, Y);
    glEnd();
}
