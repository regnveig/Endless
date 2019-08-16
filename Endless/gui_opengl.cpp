#include "gui_opengl.h"

/* ------ GUI_OPENGL ------ */

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
   glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();

   glOrtho(-1.5, 1.5, -1.5, 1.5, -20.0, 20.0);
   glRotatef(yAxisRotation, 0.0, 1.0, 0.0);
   glRotatef(xAxisRotation, 1.0, 0.0, 0.0);

   //DrawSky();

   for (auto &item : cyclone_data) {

           QVector3D vect = item.vect;
           GLfloat Radius = GLfloat(item.power * 3);
           if (Radius >= 0)
               Sky::DoStar(vect, Radius);
           else Sky::DoSun(vect, qAbs(Radius));
    }
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

void GUI_OpenGL::CelestialData(QList<celestial_data> data) {

    sky_data = data;
    this->update();
};

void GUI_OpenGL::WeatherData(QList<weather_data> data) {

    cyclone_data = data;
}

void GUI_OpenGL::StarsData(QList<star> data) {

    stars_data = data;
};

void GUI_OpenGL::DrawSky() {

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    GLfloat Z = 0.0f;

    for (auto item = 0; item < sky_data.size(); item++)
        if (sky_data.at(item).name == QString("sun"))
            Z = GLfloat(sky_data.at(item).vect1.z());

    for (auto item = 0; item < stars_data.size(); item++) {

        if (stars_data.at(item).coord.z() > Sky::SKY_BOTTOM) {

            QVector3D vect = stars_data.at(item).coord;

            if (stars_data.at(item).type == 0)
                Sky::DoStar(vect, 0.02f);
            if (stars_data.at(item).type == 1)
                Sky::DoStar(vect, 0.06f, 0.02f, 6);
            if (stars_data.at(item).type == 2)
                Sky::DoStar(vect, 0.09f, 0.03f, 8);
        }
    }

    Sky::DoSkyBox(Z);

    for (auto item = 0; item < sky_data.size(); item++) {

        if (sky_data.at(item).vect1.z() > Sky::SKY_BOTTOM) {

            QVector3D vect = sky_data.at(item).vect1;
            GLfloat Radius = GLfloat(sky_data.at(item).angular_size * 2 * M_PI);

            if (sky_data.at(item).name == QString("sun")) {

                Sky::DoSunCorona(vect, Radius * 3, Z);
                Sky::DoSun(vect, Radius);
            }
            if (sky_data.at(item).name == QString("moon")) {

                Sky::DoMoon(vect, Radius, Z, 0.0, 0.0);
            }
        }
    }
}

/* ------ SKY ------ */

void GUI_OpenGL::Sky::DoMoon(QVector3D vect, GLfloat Radius, GLfloat Z,
                             [[maybe_unused]] qreal Phase,
                             [[maybe_unused]] qreal SunDirection) {

    // VARIABLE

    // shape

    const GLfloat           cx                  = GLfloat(qSin(M_PI/3));
    const GLfloat           cy                  = GLfloat(qCos(M_PI/3));
    const GLfloat           tri_center          = GLfloat(1 / qSqrt(3));

    GLfloat                 c1                  = Radius * 0.9f;
   [[maybe_unused]] GLfloat c2                  = Radius;
    GLfloat                 ct                  = c1 * tri_center;
    GLfloat                 cs                  = Radius * 0.1f;
    GLfloat                 cm                  = Radius * 0.25f;

    // color

    GLfloat             moon_edge_color[]       = {0.97f, 0.96f, 0.9f, 0.5f};
    GLfloat             moon_dot_color[]        = {0.25f, 0.2f, 0.2f, 0.3f};
    GLfloat             moon_color[]            = {0.97f, 0.96f, 0.9f, 1.0f};
    GLfloat             moon_shadow_color[]     = {0.15f, 0.1f, 0.1f, 0.8f};
    GLfloat             frontier                = 0.3f;
    GLfloat             alpha                   = 1.0f;

    if (Z < -frontier)
        alpha = 1.0f;
    if ((Z > -frontier) && (Z < frontier))
        alpha = - Z / (frontier * 2) * 0.9f + 0.55f;
    if (Z > frontier)
        alpha = 0.1f;

    moon_edge_color[3] *= alpha;
    moon_dot_color[3] *= alpha;
    moon_color[3] *= alpha;
    moon_shadow_color[3] *= alpha;

    // PRIMITIVE

    glPushMatrix();
    PlaceSkylight(&vect);


    glBegin     (GL_POLYGON);
    glColor4fv  (moon_edge_color);
    glVertex3f  (0.0f, c2, SKY_SIZE);
    glVertex3f  (c2 * cx, c2 * cy, SKY_SIZE);
    glVertex3f  (c2 * cx, -c2 * cy, SKY_SIZE);
    glVertex3f  (0.0f, -c2, SKY_SIZE);
    glVertex3f  (-c2 * cx, -c2 * cy, SKY_SIZE);
    glVertex3f  (-c2 * cx, c2 * cy, SKY_SIZE);
    glEnd       ();


    glBegin     (GL_POLYGON);
    glColor4fv  (moon_color);
    glVertex3f  (0.0f, c1, SKY_SIZE);
    glVertex3f  (c1 * cx, c1 * cy, SKY_SIZE);
    glVertex3f  (c1 * cx, -c1 * cy, SKY_SIZE);
    glVertex3f  (0.0f, -c1, SKY_SIZE);
    glVertex3f  (-c1 * cx, -c1 * cy, SKY_SIZE);
    glVertex3f  (-c1 * cx, c1 * cy, SKY_SIZE);
    glEnd       ();

    // dots

    glBegin     (GL_TRIANGLES);
    glColor4fv  (moon_dot_color);
    glVertex3f  (ct * cy + cm, ct * cx, SKY_SIZE);
    glVertex3f  ((ct - cm) * cy, (ct - cm) * cx, SKY_SIZE);
    glVertex3f  ((ct - cm) * cy, (ct + cm) * cx, SKY_SIZE);
    glEnd       ();

    glBegin     (GL_TRIANGLES);
    glColor4fv  (moon_dot_color);
    glVertex3f  (-ct * (cy + 1), ct * cx, SKY_SIZE);
    glVertex3f  (0.0f, 2 * ct * cx, SKY_SIZE);
    glVertex3f  (0.0f, 0.0f, SKY_SIZE);
    glEnd       ();

    glBegin     (GL_TRIANGLES);
    glColor4fv  (moon_dot_color);
    glVertex3f  (ct - cm, 0.0f, SKY_SIZE);
    glVertex3f  (ct + cm * cy, cm * cx, SKY_SIZE);
    glVertex3f  (ct + cm * cy, -cm * cx, SKY_SIZE);
    glEnd       ();

    glBegin     (GL_TRIANGLES);
    glColor4fv  (moon_dot_color);
    glVertex3f  (-ct + cm, 0.0f, SKY_SIZE);
    glVertex3f  (-ct - cm * cy, -cm * cx, SKY_SIZE);
    glVertex3f  (-ct - cm * cy, cm * cx, SKY_SIZE);
    glEnd       ();

    glBegin     (GL_TRIANGLES);
    glColor4fv  (moon_dot_color);
    glVertex3f  (ct * cy + cs, -ct * cx, SKY_SIZE);
    glVertex3f  ((ct - cs) * cy, -(ct + cs) * cx, SKY_SIZE);
    glVertex3f  ((ct - cs) * cy, -(ct - cs) * cx, SKY_SIZE);
    glEnd       ();

    glBegin     (GL_TRIANGLES);
    glColor4fv  (moon_dot_color);
    glVertex3f  (-ct * cy - cm, ct * cx, SKY_SIZE);
    glVertex3f  (-(ct - cm) * cy, (ct + cm) * cx, SKY_SIZE);
    glVertex3f  (-(ct - cm) * cy, (ct - cm) * cx, SKY_SIZE);
    glEnd       ();

    // shadow

    /*
    glBegin(GL_POLYGON);
    glColor4fv (moon_shadow_color);
    glVertex3f((Radius * 0.68f * cx),
               (Radius * 0.68f * cy),
               SKY_SIZE);
    glVertex3f((Radius * 0.68f * cx),
               -(Radius * 0.68f * cy),
               SKY_SIZE);
    glVertex3f(0.0f, -(Radius * 0.8f), SKY_SIZE);
    glVertex3f(-(Radius * 0.9f * cx),
               -(Radius * 0.9f * cy),
               SKY_SIZE);
    glVertex3f(-(Radius * 0.9f * cx),
               (Radius * 0.9f * cy),
               SKY_SIZE);
    glVertex3f(0.0f, (Radius * 0.8f), SKY_SIZE);
    glEnd();
    */

    glPopMatrix();
}

void GUI_OpenGL::Sky::DoSkyBox(GLfloat Z) {

    // VARIABLE

    // color

    GLfloat             midday[]        = {0.2f, 0.48f, 0.78f, 1.0f};
    GLfloat             rise[]          = {0.88f, 0.37f, 0.07f, 1.0f};
    GLfloat             frontier        = 0.2f;
    GLfloat             frontier2       = 0.5f;
    GLfloat             lightness       = 0.0f;
    GLfloat             shine           = 0.0f;

    if (Z < -frontier)
        lightness = 0.3f;
    if ((Z > -frontier) && (Z < frontier))
        lightness = Z / (frontier * 2) * 0.7f + 0.65f;
    if (Z > frontier)
        lightness = 1.0f;

    if (Z < -frontier2)
        shine = 0.0f;
    if ((Z > -frontier2) && (Z < frontier2))
        shine = GLfloat(qCos(qreal(Z / frontier2) * M_PI_2));
    if (Z > frontier2) shine = 0.0f;

    GLfloat top[] = {midday[0] * lightness * 0.8f,
                     midday[1] * lightness * 0.8f,
                     midday[2] * lightness * 0.8f,
                     lightness};
    GLfloat bottom[] = {midday[0] * lightness * (1 - shine) + rise[0] * shine,
                        midday[1] * lightness * (1 - shine) + rise[1] * shine,
                        midday[2] * lightness * (1 - shine) + rise[2] * shine,
                        lightness * (1 - shine) + rise[3] * shine};

    GLfloat cubeColorArray[8][4] = {
        {top[0], top[1], top[2], top[3]},
        {bottom[0], bottom[1], bottom[2], bottom[3]},
        {bottom[0], bottom[1], bottom[2], bottom[3]},
        {top[0], top[1], top[2], top[3]},
        {top[0], top[1], top[2], top[3]},
        {bottom[0], bottom[1], bottom[2], bottom[3]},
        {bottom[0], bottom[1], bottom[2], bottom[3]},
        {top[0], top[1], top[2], top[3]}
    };

    // shape

    GLfloat cubeVertexArray[8][3] = {
        { SKY_SIZE,  SKY_SIZE, -SKY_SIZE},
        { SKY_SIZE, SKY_BOTTOM * SKY_SIZE, -SKY_SIZE},
        {-SKY_SIZE, SKY_BOTTOM * SKY_SIZE, -SKY_SIZE},
        {-SKY_SIZE,  SKY_SIZE, -SKY_SIZE},
        { SKY_SIZE,  SKY_SIZE,  SKY_SIZE},
        { SKY_SIZE, SKY_BOTTOM * SKY_SIZE,  SKY_SIZE},
        {-SKY_SIZE, SKY_BOTTOM * SKY_SIZE,  SKY_SIZE},
        {-SKY_SIZE,  SKY_SIZE,  SKY_SIZE}
    };

    GLubyte cubeIndexArray[6][4] = {
        {0, 3, 2, 1},
        {0, 1, 5, 4},
        {7, 4, 5, 6},
        {3, 7, 6, 2},
        {0, 4, 7, 3}
    };

    // PRIMITIVE

    glVertexPointer(3, GL_FLOAT, 0, cubeVertexArray);
    glColorPointer(4, GL_FLOAT, 0, cubeColorArray);
    glDrawElements(GL_QUADS, 24, GL_UNSIGNED_BYTE, cubeIndexArray);
}

void GUI_OpenGL::Sky::DoStar(QVector3D vect, GLfloat Radius) {

    // VARIABLE

    // color

    GLfloat         star_color[]        = {0.8f, 0.8f, 0.8f, 1.0f};

    // PRIMITIVE

    glPushMatrix();
    PlaceSkylight(&vect);

    glBegin     (GL_QUADS);
    glColor4fv  (star_color);
    glVertex3f  (0.0f, Radius, SKY_SIZE);
    glVertex3f  (Radius, 0.0f, SKY_SIZE);
    glVertex3f  (0.0f, -Radius, SKY_SIZE);
    glVertex3f  (-Radius, 0.0f, SKY_SIZE);
    glEnd       ();

    glPopMatrix();
}

void GUI_OpenGL::Sky::DoStar(QVector3D vect, GLfloat Radius, GLfloat Inner_Radius, quint8 Ray_num) {

    // VARIABLE

    // color

    GLfloat             star_color[]        = {1.0f, 1.0f, 1.0f, 1.0f};
    qreal               st                  = M_PI * 2 / Ray_num;

    // PRIMITIVE

    glPushMatrix();
    PlaceSkylight(&vect);

    for (quint8 i = 0; i < Ray_num; i++) {

        glBegin     (GL_QUADS);
        glColor4fv  (star_color);
        glVertex3f  (GLfloat(qSin((i + 1) * st)) * Inner_Radius,
                     GLfloat(qCos((i + 1) * st)) * Inner_Radius,
                     SKY_SIZE);
        glVertex3f  (0.0f, 0.0f, SKY_SIZE);
        glVertex3f  (GLfloat(qSin(i * st)) * Inner_Radius,
                     GLfloat(qCos(i * st)) * Inner_Radius,
                     SKY_SIZE);
        glVertex3f  (GLfloat(qSin((i + 0.5) * st)) * Radius,
                     GLfloat(qCos((i + 0.5) * st)) * Radius,
                     SKY_SIZE);
        glEnd       ();
    }

    glPopMatrix();
}

void GUI_OpenGL::Sky::DoSun(QVector3D vect, GLfloat Radius) {

    // VARIABLE

    // color

    GLfloat             light_color[]       = {1.0f, 1.0f, 1.0f};
    GLfloat             sun_edge_color[]    = {0.94f, 0.93f, 0.57f};

    // shape

    const GLfloat       turn                = GLfloat(qSqrt(0.5));

    GLfloat             c4                  = Radius;
    GLfloat             c3                  = c4 * turn;
    GLfloat             c2                  = Radius * 0.9f;
    GLfloat             c1                  = c2 * turn;

    // PRIMITIVE

    glPushMatrix();
    PlaceSkylight(&vect);

    glBegin     (GL_QUADS);
    glColor3fv  (sun_edge_color);
    glVertex3f  (c3, c3, SKY_SIZE);
    glVertex3f  (c3, -c3, SKY_SIZE);
    glVertex3f  (-c3, -c3, SKY_SIZE);
    glVertex3f  (-c3, c3, SKY_SIZE);
    glEnd       ();

    glBegin     (GL_QUADS);
    glColor3fv  (sun_edge_color);
    glVertex3f  (0.0f, c4, SKY_SIZE);
    glVertex3f  (c4, 0.0f, SKY_SIZE);
    glVertex3f  (0.0f, -c4, SKY_SIZE);
    glVertex3f  (-c4, 0.0f, SKY_SIZE);
    glEnd       ();

    glBegin     (GL_QUADS);
    glColor3fv  (light_color);
    glVertex3f  (c1, c1, SKY_SIZE);
    glVertex3f  (c1, -c1, SKY_SIZE);
    glVertex3f  (-c1, -c1, SKY_SIZE);
    glVertex3f  (-c1, c1, SKY_SIZE);
    glEnd       ();

    glBegin     (GL_QUADS);
    glColor3fv  (light_color);
    glVertex3f  (0.0f, c2, SKY_SIZE);
    glVertex3f  (c2, 0.0f, SKY_SIZE);
    glVertex3f  (0.0f, -c2, SKY_SIZE);
    glVertex3f  (-c2, 0.0f, SKY_SIZE);
    glEnd       ();

    glPopMatrix();
}

void GUI_OpenGL::Sky::DoSunCorona(QVector3D vect, GLfloat Radius, GLfloat Z) {

    // VARIABLE

    //color

    GLfloat             sky_color[]         = {1.0f, 1.0f, 1.0f, 0.0f};
    GLfloat             light_color1[]      = {1.0f, 1.0f, 1.0f, 0.8f};

    GLfloat             frontier2       = 0.25f;
    GLfloat             shine           = 0.0f;

    if (Z < -frontier2)
        shine = 1.0f;
    if ((Z > -frontier2) && (Z < frontier2))
        shine = 1.0f - GLfloat(qCos(qreal(Z / frontier2) * M_PI_2));
    if (Z > frontier2) shine = 1.0f;

    light_color1[3] *= shine;

    // shape

    const GLfloat       turn                = GLfloat(qSqrt(0.5));

    GLfloat             c2                  = Radius;
    GLfloat             c1                  = Radius * turn;

    // PRIMITIVE

    glPushMatrix();
    PlaceSkylight(&vect);

    glBegin     (GL_TRIANGLES);
    glColor4fv  (sky_color);
    glVertex3f  (0.0f, c2, SKY_SIZE);
    glVertex3f  (c1, c1, SKY_SIZE);
    glColor4fv  (light_color1);
    glVertex3f  (0.0f, 0.0f, SKY_SIZE);
    glEnd       ();

    glBegin     (GL_TRIANGLES);
    glColor4fv  (sky_color);
    glVertex3f  (-c1, c1, SKY_SIZE);
    glVertex3f  (0.0f, c2, SKY_SIZE);
    glColor4fv  (light_color1);
    glVertex3f  (0.0f, 0.0f, SKY_SIZE);
    glEnd       ();

    glBegin     (GL_TRIANGLES);
    glColor4fv  (sky_color);
    glVertex3f  (-c2, 0.0f, SKY_SIZE);
    glVertex3f  (-c1, c1, SKY_SIZE);
    glColor4fv  (light_color1);
    glVertex3f  (0.0f, 0.0f, SKY_SIZE);
    glEnd       ();

    glBegin     (GL_TRIANGLES);
    glColor4fv  (sky_color);
    glVertex3f  (-c1, -c1, SKY_SIZE);
    glVertex3f  (-c2, 0.0f, SKY_SIZE);
    glColor4fv  (light_color1);
    glVertex3f  (0.0f, 0.0f, SKY_SIZE);
    glEnd       ();

    glBegin     (GL_TRIANGLES);
    glColor4fv  (sky_color);
    glVertex3f  (0.0f, -c2, SKY_SIZE);
    glVertex3f  (-c1, -c1, SKY_SIZE);
    glColor4fv  (light_color1);
    glVertex3f  (0.0f, 0.0f, SKY_SIZE);
    glEnd       ();

    glBegin     (GL_TRIANGLES);
    glColor4fv  (sky_color);
    glVertex3f  (c1, -c1, SKY_SIZE);
    glVertex3f  (0.0f, -c2, SKY_SIZE);
    glColor4fv  (light_color1);
    glVertex3f  (0.0f, 0.0f, SKY_SIZE);
    glEnd       ();

    glBegin     (GL_TRIANGLES);
    glColor4fv  (sky_color);
    glVertex3f  (c2, 0.0f, SKY_SIZE);
    glVertex3f  (c1, -c1, SKY_SIZE);
    glColor4fv  (light_color1);
    glVertex3f  (0.0f, 0.0f, SKY_SIZE);
    glEnd       ();

    glBegin     (GL_TRIANGLES);
    glColor4fv  (sky_color);
    glVertex3f  (c1, c1, SKY_SIZE);
    glVertex3f  (c2, 0.0f, SKY_SIZE);
    glColor4fv  (light_color1);
    glVertex3f  (0.0f, 0.0f, SKY_SIZE);
    glEnd();

    glPopMatrix();
}

void GUI_OpenGL::Sky::PlaceSkylight(QVector3D *vect) {

    GLfloat zenith = GLfloat(qRadiansToDegrees(qAcos(qreal(vect->z()))));
    GLfloat azimuth = GLfloat(qRadiansToDegrees(((vect->x() < 0) ? (-1) : 1)
                      * qAcos(qreal(vect->y()) / qSqrt(qPow(qreal(vect->x()), 2) + qPow(qreal(vect->y()), 2)))));

    glRotatef   (-90.0, 1.0, 0.0, 0.0);
    glRotatef   (90 - azimuth, 0.0, 0.0, 1.0);
    glRotatef   (zenith, 0.0f, 1.0f, 0.0f);
}
