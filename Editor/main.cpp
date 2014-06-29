#define GLEW_STATIC
#include <GL/glew.h>

#include <QApplication>

#include "maineditorwindow.h"
#include "Rendering\RenderManager.h"
#include "Scene\ResourceManager.h"
#include "Scene\Scene.h"


int main(int argc, char *argv[])
{
    // QT setup
    QApplication a(argc, argv);
    MainEditorWindow w;
    w.show();

    RenderManager::Singleton().Startup(0);
    ResourceManager::Singleton().Startup("..\\Game\\Assets\\Resources.xml");        // TODO fix these paths!!
    Scene scene("..\\Game\\Assets\\Scenes\\Scene0.xml");

    w.SetRoot(scene.GetRootObject());

    return a.exec();

}

