#pragma once

#include <string>
#include <tinyxml2.h>

using std::string;

class Scene;

class GameProject
{
public:
    GameProject();

    bool    New(string filename = "");
    bool    Load(string filename);
    bool    Save(string filename = "");
    bool    Unload();

    string  GetName();
    void    SetName(string name);

    void    GetResolution(int& width, int& height);
    void    SetResolution(int width, int height);

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
};