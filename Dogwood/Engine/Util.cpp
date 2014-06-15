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