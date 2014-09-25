#include "Util.h"

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

XMLNode* WriteFloatToXML(float value, string nodeName, string attribName, XMLDocument& doc)
{
    XMLElement* xmlnode = doc.NewElement(nodeName.c_str());

    // There doesn't seem to be a way to set float precision in tinyxml2,
    // so write the float to a string with the desried precision instead
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