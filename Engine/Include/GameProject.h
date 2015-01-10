#pragma once

#include <string>
#include <tinyxml2.h>

using std::string;

class Scene;

class GameProject
{
public:
    static GameProject& Singleton()
    {
        static GameProject singleton;
        return singleton;
    }
    GameProject() {}

    void    Startup();
    void    Shutdown();

    bool    New(string filename = "");
    bool    Load(string filename);
    bool    Save(string filename = "");
    bool    Unload();
    bool    IsLoaded();

    string  GetName();
    void    SetName(string name);

    void    GetResolution(int& width, int& height);
    void    SetResolution(int width, int height);

    string  GetResourceBasePath();
    void    SetResourceBasePath(string path);

    void    AddScene(Scene* scene);
    void    RemoveScene(Scene* scene);

private:
    void    LoadSettings(tinyxml2::XMLElement* settingsXML);
    void    LoadSceneList();

    void    SerializeSettings(tinyxml2::XMLDocument& rootDoc, tinyxml2::XMLElement* parent);
    void    SerilaizeSceneList();

    bool    m_loaded;
    string  m_filename;
    string  m_name;

    int     m_width;
    int     m_height;

    string  m_resourceDir;
};