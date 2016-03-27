#pragma once

#include <string>
#include "tinyxml2.h"

using std::string;
using namespace tinyxml2;

class HierarchicalSerializer
{
public:

    class Element
    {
    public:
        void SetAttribute(string name, int value);
        void SetAttribute(string name, float value);
        void SetAttribute(string name, bool value);
        void SetAttribute(string name, string value);

    private:
        XMLElement* m_element;
    };

    HierarchicalSerializer();

    Element*    NewElement(string name);

    void        Save(string filepath);
    void        Load(string filepath);

private:
    XMLDocument     m_document;
};