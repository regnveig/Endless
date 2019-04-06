#include "global.h"
#include "gui_opengl.h"
#include "sky.h"

int main(int argc, char *argv[]) {

    QApplication a(argc, argv);
    GUI_OpenGL w;
    Sky *sky = new Sky();
    w.show();
    Getter getter;

    //a.connect(sky, SIGNAL(Data(QList<celestial_data>)), &getter, SLOT(GetData(QList<celestial_data>)));
    a.connect(sky, SIGNAL(Data(QList<celestial_data>)),
                 &w, SLOT(SkyData(QList<celestial_data>)));

    return a.exec();
}
