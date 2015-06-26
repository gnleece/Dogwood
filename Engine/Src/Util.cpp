#include "Util.h"

#include <climits>
#include <ctime>
#include <fstream>
#include <ostream>

using namespace tinyxml2;

unsigned int MakeGuid(string str)
{
    // Get timestamp
    time_t timer;
    time(&timer);
    string timestamp = std::to_string(timer);

    // Generate a random number as well, in case this function was called so recently
    // that we have the same timestamp as before
    int random = rand() % INT_MAX;

    // Hash string + timestamp + random number to create unique ID
    unsigned int guid = std::hash<string>()(str + timestamp + std::to_string(random));

    return guid;
}

Vector3 ReadVector3FromXML(XMLElement* xmlnode)
{
    Vector3 v;
    if (xmlnode)
    {
        v[0] = xmlnode->FloatAttribute("x");
        v[1] = xmlnode->FloatAttribute("y");
        v[2] = xmlnode->FloatAttribute("z");
    }
    return v;
}

XMLNode* WriteVector3ToXML(Vector3 v, string name, XMLDocument& doc)
{
    XMLElement* xmlnode = doc.NewElement(name.c_str());

    // There doesn't seem to be a way to set float precision in tinyxml2,
    // so write the float to a string with the desried precision instead
    char buf[20] = { 0 };
    sprintf_s(buf, "%1.5f", v[0]);
    xmlnode->SetAttribute("x", buf);
    sprintf_s(buf, "%1.5f", v[1]);
    xmlnode->SetAttribute("y", buf);
    sprintf_s(buf, "%1.5f", v[2]);
    xmlnode->SetAttribute("z", buf);

    return xmlnode;
}

ColourRGB ReadColourFromXML(XMLElement* xmlnode)
{
    ColourRGB c;
    if (xmlnode)
    {
        c.r = xmlnode->FloatAttribute("r");
        c.g = xmlnode->FloatAttribute("g");
        c.b = xmlnode->FloatAttribute("b");
    }
    return c;
}

XMLNode* WriteColourToXML(ColourRGB c, string name, XMLDocument& doc)
{
    XMLElement* xmlnode = doc.NewElement(name.c_str());

    // There doesn't seem to be a way to set float precision in tinyxml2,
    // so write the float to a string with the desried precision instead
    char buf[20] = { 0 };
    sprintf_s(buf, "%1.5f", c.r);
    xmlnode->SetAttribute("r", buf);
    sprintf_s(buf, "%1.5f", c.g);
    xmlnode->SetAttribute("g", buf);
    sprintf_s(buf, "%1.5f", c.b);
    xmlnode->SetAttribute("b", buf);

    return xmlnode;
}

XMLNode* WriteIntToXML(int value, string nodeName, string attribName, XMLDocument& doc)
{
    XMLElement* xmlnode = doc.NewElement(nodeName.c_str());
    xmlnode->SetAttribute(attribName.c_str(), value);
    return xmlnode;
}

XMLNode* WriteUnsignedIntToXML(unsigned int value, string nodeName, string attribName, XMLDocument& doc)
{
    XMLElement* xmlnode = doc.NewElement(nodeName.c_str());
    xmlnode->SetAttribute(attribName.c_str(), value);
    return xmlnode;
}

XMLNode* WriteFloatToXML(float value, string nodeName, string attribName, XMLDocument& doc)
{
    XMLElement* xmlnode = doc.NewElement(nodeName.c_str());

    // There doesn't seem to be a way to set float precision in tinyxml2,
    // so write the float to a string with the desiried precision instead
    char buf[20] = { 0 };
    sprintf_s(buf, "%1.5f", value);
    xmlnode->SetAttribute(attribName.c_str(), buf);

    return xmlnode;
}

XMLNode* WriteStringToXML(string str, string nodeName, string attribName, XMLDocument& doc)
{
    XMLElement* xmlnode = doc.NewElement(nodeName.c_str());
    xmlnode->SetAttribute(attribName.c_str(), str.c_str());
    return xmlnode;
}

void FileCopy(string source, string destination)
{
    std::ifstream src(source.c_str(), std::ios::binary);
    std::ofstream dst(destination.c_str(), std::ios::binary);

    dst << src.rdbuf();
}
