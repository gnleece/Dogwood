#define GLEW_STATIC
#include <GL/glew.h>

#include <QApplication>
#include <QTime>

#include "HierarchyModel.h"
#include "maineditorwindow.h"
#include "Rendering\RenderManager.h"
#include "Scene\ResourceManager.h"
#include "Scene\Scene.h"


int main(int argc, char *argv[])
{
    // QT setup
    QApplication app(argc, argv);
    MainEditorWindow window;
    window.show();

    RenderConfig renderConfig;
    renderConfig.width = 640;
    renderConfig.height = 480;
    RenderManager::Singleton().Startup(renderConfig);
    
    ResourceManager::Singleton().Startup("..\\Game\\Assets\\Resources.xml");        // TODO fix these paths!!
    Scene scene("..\\Game\\Assets\\Scenes\\Scene0.xml");

    HierarchyModel* model = new HierarchyModel(scene.GetRootObject());
    window.SetHierarchyModel(model);

    QTime lastUpdate;
    int updateTimeStep = 1000 / 60;
    while (true)        // TODO handle quit / close window
    {
        app.processEvents();
        
        QTime currentTime = QTime::currentTime();
        int timeSinceLastUpdate = lastUpdate.msecsTo(currentTime);
        while (timeSinceLastUpdate > updateTimeStep)
        {
            // DO GAME UPDATE
            timeSinceLastUpdate -= updateTimeStep;
            lastUpdate = lastUpdate.addMSecs(updateTimeStep);
        }

        RenderManager::Singleton().RenderScene(scene.GetRootObject());
        window.Paint();
    }
    app.exit();

}

