#pragma once

#define GLEW_STATIC
#include <GL/glew.h>
#include <tinyxml2.h>
#include <string>

#include "Math\Algebra.h"
#include "Rendering\Colour.h"

using namespace tinyxml2;
using std::string;

Vector3     ReadVector3FromXML(XMLElement* xmlnode);
XMLNode*    WriteVector3ToXML(Vector3 vector, string name, XMLDocument& doc);

ColourRGB   ReadColourFromXML(XMLElement* xmlnode);
XMLNode*    WriteColourToXML(ColourRGB colour, string name, XMLDocument& doc);

XMLNode*    WriteFloatToXML(float value, string nodeName, string attribName, XMLDocument& doc);
XMLNode*    WriteStringToXML(string str, string nodeName, string attribName, XMLDocument& doc);