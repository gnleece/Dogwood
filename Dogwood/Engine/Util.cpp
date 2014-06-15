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