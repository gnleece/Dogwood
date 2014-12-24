#pragma once

#include <string>
#include <tinyxml2.h>

using std::string;

class GameProject
{
public:
    GameProject();

    bool    New(string filepath = "");
    bool    Load(string filepath);
    bool    Save(string filepath = "");
    bool    Unload();

    string  GetName();
    void    SetName(string name);

    void    GetResolution(int& width, int& height);
    void    SetResolution(int width, int height);

private:
    void    LoadSettings(tinyxml2::XMLElement* settingsXML);

    bool    m_loaded;
    string  m_filepath;
    string  m_name;

    int     m_width;
    int     m_height;
};