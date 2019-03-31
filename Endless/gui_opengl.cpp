#include "gui_opengl.h"

GUI_OpenGL::GUI_OpenGL(QWidget *parent) : QGLWidget(parent) {

    resize(300,300);
}

void GUI_OpenGL::initializeGL() {

   qglClearColor(Qt::black);                            // заполняем экран белым цветом
   glEnable(GL_DEPTH_TEST);                             // задаем глубину проверки пикселей
   glShadeModel(GL_FLAT);                               // убираем режим сглаживания цветов
   glEnable(GL_CULL_FACE);                              // говорим, что будем строить только внешние поверхности
   glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);            // фигуры будут закрашены с обеих сторон
}

void GUI_OpenGL::resizeGL(int nWidth, int nHeight) {

    glViewport(0, 0, nHeight, nHeight);                 // установка точки обзора
    glMatrixMode(GL_PROJECTION);                        // установка режима матрицы
    glLoadIdentity();                                   // загрузка матрицы
}

void GUI_OpenGL::paintGL() {

   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // очистка экрана
   glMatrixMode(GL_MODELVIEW);                          // задаем модельно-видовую матрицу
   glLoadIdentity();                                    // загрузка единичную матрицу

   QColor halfGreen(0, 128, 0, 255);                    // устанавливаем цвет квадрата
   qglColor(halfGreen);                                 // задаем цвет
   glBegin(GL_QUADS);                                   // говорим, что рисовать будем прямоугольник

   // задаем вершины многоугольника

   glVertex3f(0.5, 0.5, 0.5);
   glVertex3f(-0.5, 0.5, 0.5);
   glVertex3f(-0.5, -0.5, 0.5);
   glVertex3f(0.5, -0.5, 0.5);
   glEnd();
}
