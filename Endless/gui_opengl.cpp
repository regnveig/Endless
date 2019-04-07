#include "gui_opengl.h"

GUI_OpenGL::GUI_OpenGL(QWidget *parent) : QOpenGLWidget(parent) {

    xAxisRotation = yAxisRotation = 0;
    resize(800,800);
}

void GUI_OpenGL::initializeGL() {

   glShadeModel(GL_SMOOTH);
   glEnable(GL_CULL_FACE);
   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
   glEnable(GL_BLEND);
   glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
   glEnableClientState(GL_VERTEX_ARRAY);
   glEnableClientState(GL_COLOR_ARRAY);
}

void GUI_OpenGL::resizeGL(int nWidth, int nHeight) {

    glViewport(0, 0, nHeight, nHeight);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, 0, GLint(nWidth), GLint(nHeight));

    currentWidth = nWidth;
    currentHeight = nHeight;
}

void GUI_OpenGL::paintGL() {

   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glClearColor(0.0f, 0.02f, 0.0f, 1.0f);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();

   glOrtho(-1.5, 1.5, -1.5, 1.5, -20.0, 20.0);
   glRotatef(yAxisRotation, 0.0, 1.0, 0.0);
   glRotatef(xAxisRotation, 1.0, 0.0, 0.0);

   // Sky

   Sky::SkyBox();

   for (auto item = 0; item < sky_data.size(); item++) {
       QVector3D vect = sky_data.at(item).vect1;
       GLfloat Radius = GLfloat(sky_data.at(item).angular_size * 2 * M_PI);
       if (sky_data.at(item).name == QString("sun")) {
           Sky::DoSunCorona(vect, Radius * 3);
           Sky::DoSun(vect, Radius); }
       if (sky_data.at(item).name == QString("moon")) {
           Sky::DoMoon(vect, Radius, 0.0, 0.0); }
       if (sky_data.at(item).name == QString("star_0")) {
           Sky::DoStar(vect, 0.02f); }
       if (sky_data.at(item).name == QString("star_1")) {
           Sky::DoStar(vect, 0.06f, 0.02f, 6); }
       if (sky_data.at(item).name == QString("star_2")) {
           Sky::DoStar(vect, 0.09f, 0.03f, 8); }
   }

   //Sky::DoGround();
}

void GUI_OpenGL::mousePressEvent(QMouseEvent *event) {

    pressPosition = event->pos();
}

void GUI_OpenGL::mouseMoveEvent(QMouseEvent *event) {

    xAxisRotation += (180 * (GLfloat(event->y()) - GLfloat(pressPosition.y()))) / (currentHeight);
    yAxisRotation += (180 * (GLfloat(event->x()) - GLfloat(pressPosition.x()))) / (currentWidth);
    pressPosition = event->pos();
    this->update();
}

void GUI_OpenGL::Sky::SkyBox() {

    // GLfloat sky_blue[] = {0.2f, 0.48f, 0.78f};
    // GLfloat hell[] = {0.88f, 0.37f, 0.07f};
    GLfloat cz = 5.0f;

    GLfloat cubeColorArray[8][3] = {
        {0.2f, 0.48f, 0.78f},             // Sky
        {0.88f, 0.37f, 0.07f},
        {0.88f, 0.37f, 0.07f},
        {0.2f, 0.48f, 0.78f},             // Sky
        {0.2f, 0.48f, 0.78f},             // Sky
        {0.88f, 0.37f, 0.07f},
        {0.88f, 0.37f, 0.07f},
        {0.2f, 0.48f, 0.78f}              // Sky
    };

    // Вот это лучше не трогать. Это древняя магия и она работает

    GLfloat cubeVertexArray[8][3] = {
        { cz,  cz, -cz},
        { cz, -cz, -cz},
        {-cz, -cz, -cz},
        {-cz,  cz, -cz},
        { cz,  cz,  cz},
        { cz, -cz,  cz},
        {-cz, -cz,  cz},
        {-cz,  cz,  cz}
    };

    GLubyte cubeIndexArray[6][4] = {
        {0, 3, 2, 1},
        {0, 1, 5, 4},
        {7, 4, 5, 6},
        {3, 7, 6, 2},
        {1, 2, 6, 5},
        {0, 4, 7, 3}
    };

    // Магия закончилась

    glVertexPointer(3, GL_FLOAT, 0, cubeVertexArray);
    glColorPointer(3, GL_FLOAT, 0, cubeColorArray);
    glDrawElements(GL_QUADS, 24, GL_UNSIGNED_BYTE, cubeIndexArray);

}

void GUI_OpenGL::Sky::DoGround() {

    GLfloat cz = 5.0f;

    glBegin     (GL_QUADS);
    glColor3f  (0.14f, 0.49f, 0.18f);
    glVertex3f  (cz, -0.5, cz);
    glVertex3f  (cz, -0.5, -cz);
    glVertex3f  (-cz, -0.5, -cz);
    glVertex3f  (-cz, -0.5, cz);
    glEnd       ();
}

void GUI_OpenGL::Sky::DoSun(QVector3D vect, GLfloat Radius) {

    // VARIABLE

    GLfloat light_color[]       = {1.0f, 1.0f, 1.0f};
    GLfloat sun_edge_color[]    = {0.94f, 0.93f, 0.57f};

    const GLfloat turn          = GLfloat(qSqrt(0.5));

    GLfloat far                 = 5.0f;
    GLfloat c4                  = Radius;
    GLfloat c3                  = c4 * turn;
    GLfloat c2                  = Radius * 0.9f;
    GLfloat c1                  = c2 * turn;

    // PRIMITIVE

    glPushMatrix();
    PlaceSkylight(&vect);

    glBegin     (GL_QUADS);
    glColor3fv  (sun_edge_color);
    glVertex3f  (c3, c3, far);
    glVertex3f  (c3, -c3, far);
    glVertex3f  (-c3, -c3, far);
    glVertex3f  (-c3, c3, far);
    glEnd       ();

    glBegin     (GL_QUADS);
    glColor3fv  (sun_edge_color);
    glVertex3f  (0.0f, c4, far);
    glVertex3f  (c4, 0.0f, far);
    glVertex3f  (0.0f, -c4, far);
    glVertex3f  (-c4, 0.0f, far);
    glEnd       ();

    glBegin     (GL_QUADS);
    glColor3fv  (light_color);
    glVertex3f  (c1, c1, far);
    glVertex3f  (c1, -c1, far);
    glVertex3f  (-c1, -c1, far);
    glVertex3f  (-c1, c1, far);
    glEnd       ();

    glBegin     (GL_QUADS);
    glColor3fv  (light_color);
    glVertex3f  (0.0f, c2, far);
    glVertex3f  (c2, 0.0f, far);
    glVertex3f  (0.0f, -c2, far);
    glVertex3f  (-c2, 0.0f, far);
    glEnd       ();

    glPopMatrix();
}

void GUI_OpenGL::Sky::DoMoon(QVector3D vect, GLfloat Radius, [[maybe_unused]] qreal Phase, [[maybe_unused]] qreal SunDirection) {

    // VARIABLE

    GLfloat moon_edge_color[]   = {0.25f, 0.2f, 0.2f};
    GLfloat moon_dot_color[]   = {0.7f, 0.65f, 0.65f};
    GLfloat moon_color[]        = {0.97f, 0.95f, 0.95f};
    GLfloat moon_shadow_color[]   = {0.15f, 0.1f, 0.1f, 0.8f}; // использовать текущий цвет неба?

    const GLfloat cx            = GLfloat(qSin(M_PI/3));
    const GLfloat cy            = GLfloat(qCos(M_PI/3));
    const GLfloat tri_center    = GLfloat(1 / qSqrt(3));

    GLfloat far                 = 5.0f;
    GLfloat c1                  = Radius * 0.9f;
    GLfloat c2                  = Radius;
    GLfloat ct                  = c1 * tri_center;
    GLfloat cs                  = Radius * 0.1f;
    GLfloat cm                  = Radius * 0.25f;

    // PRIMITIVE

    glPushMatrix();
    PlaceSkylight(&vect);

    glBegin     (GL_POLYGON);
    glColor3fv  (moon_edge_color);
    glVertex3f  (0.0f, c2, far);
    glVertex3f  (c2 * cx, c2 * cy, far);
    glVertex3f  (c2 * cx, -c2 * cy, far);
    glVertex3f  (0.0f, -c2, far);
    glVertex3f  (-c2 * cx, -c2 * cy, far);
    glVertex3f  (-c2 * cx, c2 * cy, far);
    glEnd       ();

    glBegin     (GL_POLYGON);
    glColor3fv  (moon_color);
    glVertex3f  (0.0f, c1, far);
    glVertex3f  (c1 * cx, c1 * cy, far);
    glVertex3f  (c1 * cx, -c1 * cy, far);
    glVertex3f  (0.0f, -c1, far);
    glVertex3f  (-c1 * cx, -c1 * cy, far);
    glVertex3f  (-c1 * cx, c1 * cy, far);
    glEnd       ();

    // dots

    glBegin     (GL_TRIANGLES);
    glColor3fv  (moon_dot_color);
    glVertex3f  (ct * cy + cm, ct * cx, far);
    glVertex3f  ((ct - cm) * cy, (ct - cm) * cx, far);
    glVertex3f  ((ct - cm) * cy, (ct + cm) * cx, far);
    glEnd       ();

    glBegin     (GL_TRIANGLES);
    glColor3fv  (moon_dot_color);
    glVertex3f  (-ct * (cy + 1), ct * cx, far);
    glVertex3f  (0.0f, 2 * ct * cx, far);
    glVertex3f  (0.0f, 0.0f, far);
    glEnd       ();

    glBegin     (GL_TRIANGLES);
    glColor3fv  (moon_dot_color);
    glVertex3f  (ct - cm, 0.0f, far);
    glVertex3f  (ct + cm * cy, cm * cx, far);
    glVertex3f  (ct + cm * cy, -cm * cx, far);
    glEnd       ();

    glBegin     (GL_TRIANGLES);
    glColor3fv  (moon_dot_color);
    glVertex3f  (-ct + cm, 0.0f, far);
    glVertex3f  (-ct - cm * cy, -cm * cx, far);
    glVertex3f  (-ct - cm * cy, cm * cx, far);
    glEnd       ();

    glBegin     (GL_TRIANGLES);
    glColor3fv  (moon_dot_color);
    glVertex3f  (ct * cy + cs, -ct * cx, far);
    glVertex3f  ((ct - cs) * cy, -(ct + cs) * cx, far);
    glVertex3f  ((ct - cs) * cy, -(ct - cs) * cx, far);
    glEnd       ();

    glBegin     (GL_TRIANGLES);
    glColor3fv  (moon_edge_color);
    glVertex3f  (-ct * cy - cm, ct * cx, far);
    glVertex3f  (-(ct - cm) * cy, (ct + cm) * cx, far);
    glVertex3f  (-(ct - cm) * cy, (ct - cm) * cx, far);
    glEnd       ();

    // shadow

    glBegin(GL_POLYGON);
    glColor4fv (moon_shadow_color);
    glVertex3f((Radius * 0.68f * cx),
               (Radius * 0.68f * cy),
               far);
    glVertex3f((Radius * 0.68f * cx),
               -(Radius * 0.68f * cy),
               far);
    glVertex3f(0.0f, -(Radius * 0.8f), far);
    glVertex3f(-(Radius * 0.9f * cx),
               -(Radius * 0.9f * cy),
               far);
    glVertex3f(-(Radius * 0.9f * cx),
               (Radius * 0.9f * cy),
               far);
    glVertex3f(0.0f, (Radius * 0.8f), far);
    glEnd();

    glPopMatrix();
}

void GUI_OpenGL::Sky::DoStar(QVector3D vect, GLfloat Radius) {

    // VARIABLE

    GLfloat star_color[]        = {1.0f, 1.0f, 1.0f};
    GLfloat far                 = 5.0f;

    // PRIMITIVE

    glPushMatrix();
    PlaceSkylight(&vect);

    glBegin     (GL_QUADS);
    glColor3fv  (star_color);
    glVertex3f  (0.0f, Radius, far);
    glVertex3f  (Radius, 0.0f, far);
    glVertex3f  (0.0f, -Radius, far);
    glVertex3f  (-Radius, 0.0f, far);
    glEnd       ();

    glPopMatrix();
}

void GUI_OpenGL::Sky::DoStar(QVector3D vect, GLfloat Radius, GLfloat Inner_Radius, quint8 Ray_num) {

    // VARIABLE

    GLfloat star_color[]        = {1.0f, 1.0f, 1.0f};
    qreal st                    = M_PI * 2 / Ray_num;
    GLfloat far                 = 5.0f;

    // PRIMITIVE

    glPushMatrix();
    PlaceSkylight(&vect);

    for (quint8 i = 0; i < Ray_num; i++) {

        glBegin     (GL_QUADS);
        glColor3fv  (star_color);
        glVertex3f  (GLfloat(qSin((i + 1) * st)) * Inner_Radius,
                     GLfloat(qCos((i + 1) * st)) * Inner_Radius,
                     far);
        glVertex3f  (0.0f, 0.0f, far);
        glVertex3f  (GLfloat(qSin(i * st)) * Inner_Radius,
                     GLfloat(qCos(i * st)) * Inner_Radius,
                     far);
        glVertex3f  (GLfloat(qSin((i + 0.5) * st)) * Radius,
                     GLfloat(qCos((i + 0.5) * st)) * Radius,
                     far);
        glEnd       ();
    }

    glPopMatrix();
}

void GUI_OpenGL::Sky::DoSunCorona(QVector3D vect, GLfloat Radius) {

    // VARIABLE

    GLfloat sky_color[]  = {0.9f, 0.9f, 0.9f, 0.0f};
    GLfloat light_color1[]  = {0.9f, 0.9f, 0.9f, 1.0f};

    const GLfloat turn      = GLfloat(qSqrt(0.5));

    GLfloat far             = 5.0f;
    GLfloat c2              = Radius;
    GLfloat c1              = Radius * turn;

    // PRIMITIVE

    glPushMatrix();
    PlaceSkylight(&vect);

    glBegin     (GL_TRIANGLES);
    glColor4fv  (sky_color);
    glVertex3f  (0.0f, c2, far);
    glVertex3f  (c1, c1, far);
    glColor4fv  (light_color1);
    glVertex3f  (0.0f, 0.0f, far);
    glEnd       ();

    glBegin     (GL_TRIANGLES);
    glColor4fv  (sky_color);
    glVertex3f  (-c1, c1, far);
    glVertex3f  (0.0f, c2, far);
    glColor4fv  (light_color1);
    glVertex3f  (0.0f, 0.0f, far);
    glEnd       ();

    glBegin     (GL_TRIANGLES);
    glColor4fv  (sky_color);
    glVertex3f  (-c2, 0.0f, far);
    glVertex3f  (-c1, c1, far);
    glColor4fv  (light_color1);
    glVertex3f  (0.0f, 0.0f, far);
    glEnd       ();

    glBegin     (GL_TRIANGLES);
    glColor4fv  (sky_color);
    glVertex3f  (-c1, -c1, far);
    glVertex3f  (-c2, 0.0f, far);
    glColor4fv  (light_color1);
    glVertex3f  (0.0f, 0.0f, far);
    glEnd       ();

    glBegin     (GL_TRIANGLES);
    glColor4fv  (sky_color);
    glVertex3f  (0.0f, -c2, far);
    glVertex3f  (-c1, -c1, far);
    glColor4fv  (light_color1);
    glVertex3f  (0.0f, 0.0f, far);
    glEnd       ();

    glBegin     (GL_TRIANGLES);
    glColor4fv  (sky_color);
    glVertex3f  (c1, -c1, far);
    glVertex3f  (0.0f, -c2, far);
    glColor4fv  (light_color1);
    glVertex3f  (0.0f, 0.0f, far);
    glEnd       ();

    glBegin     (GL_TRIANGLES);
    glColor4fv  (sky_color);
    glVertex3f  (c2, 0.0f, far);
    glVertex3f  (c1, -c1, far);
    glColor4fv  (light_color1);
    glVertex3f  (0.0f, 0.0f, far);
    glEnd       ();

    glBegin     (GL_TRIANGLES);
    glColor4fv  (sky_color);
    glVertex3f  (c1, c1, far);
    glVertex3f  (c2, 0.0f, far);
    glColor4fv  (light_color1);
    glVertex3f  (0.0f, 0.0f, far);
    glEnd();

    glPopMatrix();
}

void GUI_OpenGL::SkyData(QList<celestial_data> data) { sky_data = data; this->update(); };

void GUI_OpenGL::Sky::PlaceSkylight(QVector3D *vect) {

    GLfloat zenith = GLfloat(qRadiansToDegrees(qAcos(qreal(vect->z()))));
    GLfloat azimuth = GLfloat(qRadiansToDegrees(((vect->x() < 0) ? (-1) : 1)
                      * qAcos(qreal(vect->y()) / qSqrt(qPow(qreal(vect->x()), 2) + qPow(qreal(vect->y()), 2)))));

    glRotatef   (-90.0, 1.0, 0.0, 0.0);
    glRotatef   (90 - azimuth, 0.0, 0.0, 1.0);
    glRotatef   (zenith, 0.0f, 1.0f, 0.0f);
}
