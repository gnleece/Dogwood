#pragma once

#include <string>
#include <tinyxml2.h>

using std::string;

class GameProject
{
public:
    bool    Load(string filepath);
    bool    Save(string filepath = "");

    string  GetName();
    void    SetName(string name);

    void    GetResolution(int& width, int& height);
    void    SetResolution(int width, int height);

private:
    void    LoadSettings(tinyxml2::XMLElement* settingsXML);

    string  m_filepath;
    string  m_name;

    int     m_width;
    int     m_height;
};