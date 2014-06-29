
#include "maineditorwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    // QT setup
    QApplication a(argc, argv);
    MainEditorWindow w;
    w.show();

    /*
    RenderManager::Singleton().Startup(0);
    ResourceManager::Singleton().Startup("..\\Game\\Assets\\Resources.xml");        // TODO fix these paths!!
    Scene scene("..\\Game\\Assets\\Scenes\\Scene0.xml");
    */

    return a.exec();

}

