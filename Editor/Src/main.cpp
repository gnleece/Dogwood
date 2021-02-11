#define GLEW_STATIC
#include <GL/glew.h>

#include <ctime>
#include <QApplication>
#include <QTime>

#include "HierarchyModel.h"
#include "maineditorwindow.h"
#include "GameProject.h"
#include "Rendering\RenderManager.h"
#include "Scene\ResourceManager.h"
#include "Scene\Scene.h"

int main(int argc, char *argv[])
{
    srand(time(NULL));

    // QT setup.
    QApplication app(argc, argv);
    MainEditorWindow window;
    window.show();

    // Render setup. Must be done AFTER window (QT) setup
    int renderWidth = 990;           // TODO set this properly
    int renderHeight = 610;
    ColorRGB clearColor = ColorRGB(0.4f, 0.4f, 0.4f);   // TODO fixme
    RenderManager::Singleton()->Startup(renderWidth, renderHeight);
    
    // Other manager setup
    ResourceManager::Singleton().Startup();
    GameProject::Singleton().Startup(true);     // toolside = true

    // Post-setup step
    window.PostSetup();

    // Event & render loop
    QTime lastUpdate;
    int updateTimeStep = 1000 / 60;
    while (window.IsOpen())
    {
        app.processEvents();
        
        QTime currentTime = QTime::currentTime();
        int timeSinceLastUpdate = lastUpdate.msecsTo(currentTime);
        while (timeSinceLastUpdate > updateTimeStep)
        {
            timeSinceLastUpdate -= updateTimeStep;
            lastUpdate = lastUpdate.addMSecs(updateTimeStep);
        }

        RenderManager::Singleton()->RenderScene();
        window.Update();
    }
    app.exit();

    // Shutdown
    GameProject::Singleton().Shutdown();
    ResourceManager::Singleton().Shutdown();
    RenderManager::Singleton()->Shutdown();
}

