#pragma once

#include <string>

using std::string;

class HierarchicalSerializer
{
public:

    struct Element
    {
        void SetAttribute(string name, int value);
        void SetAttribute(string name, float value);
        void SetAttribute(string name, bool value);
        void SetAttribute(string name, string value);
    };

    HierarchicalSerializer();

    Element*    NewElement(string name);
};