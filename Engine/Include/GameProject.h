#pragma once

#include <string>

using std::string;

class GameComponentFactory;
class HierarchicalDeserializer;
class HierarchicalSerializer;
class Scene;

class GameProject
{
public:
    struct PhysicsSettings
    {
        bool    Enabled;
        float   Gravity;

        PhysicsSettings();
        PhysicsSettings(bool enabled, float gravity);
    };

    static GameProject& Singleton()
    {
        static GameProject singleton;
        return singleton;
    }
    GameProject() {}

    void    Startup(bool toolside = false);
    void    Shutdown();

    bool    New(string name, string filename, string resourcePath);
    bool    Load(string filename);
    bool    Save(string filename = "");
    bool    Unload();
    bool    IsLoaded();
    bool    IsToolside();

    void    SetRuntimeComponentFactory(GameComponentFactory* factory, bool engine);
    GameComponentFactory* GetRuntimeComponentFactory(bool engine);

    string  GetName();
    void    SetName(string name);

    string  GetFilename();

    void    GetResolution(int& width, int& height);
    void    SetResolution(int width, int height);

    string  GetResourceBasePath();
    void    SetResourceBasePath(string path);

    void    AddScene(Scene* scene);
    void    RemoveScene(Scene* scene);

    PhysicsSettings& GetPhysicsSettings();
    void    SetPhysicsSettings(PhysicsSettings& settings);

private:
    void    LoadSettings(HierarchicalDeserializer* deserializer);
    void    LoadSceneList();

    void    SerializeSettings(HierarchicalSerializer* serializer);
    void    SerilaizeSceneList();

    bool    m_loaded;
    string  m_filename;
    string  m_name;

    bool    m_toolside;

    int     m_width;
    int     m_height;

    string  m_resourceDir;

    PhysicsSettings m_physicsSettings;

    GameComponentFactory* m_gameComponentFactory;
    GameComponentFactory* m_engineComponentFactory;
};