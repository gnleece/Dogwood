#pragma once

#define GLEW_STATIC
#include <GL/glew.h>
#include <tinyxml2.h>
#include <string>

#include "Math\Algebra.h"
#include "Rendering\Colour.h"

using std::string;

unsigned int        MakeGuid(string str);

Vector3             ReadVector3FromXML(tinyxml2::XMLElement* xmlnode);
tinyxml2::XMLNode*  WriteVector3ToXML(Vector3 vector, string name, tinyxml2::XMLDocument& doc);

ColourRGB           ReadColourFromXML(tinyxml2::XMLElement* xmlnode);
tinyxml2::XMLNode*  WriteColourToXML(ColourRGB colour, string name, tinyxml2::XMLDocument& doc);

tinyxml2::XMLNode*  WriteIntToXML(int value, string nodeName, string attribName, tinyxml2::XMLDocument& doc);
tinyxml2::XMLNode*  WriteUnsignedIntToXML(unsigned int value, string nodeName, string attribName, tinyxml2::XMLDocument& doc);
tinyxml2::XMLNode*  WriteFloatToXML(float value, string nodeName, string attribName, tinyxml2::XMLDocument& doc);
tinyxml2::XMLNode*  WriteStringToXML(string str, string nodeName, string attribName, tinyxml2::XMLDocument& doc);

void                FileCopy(string source, string destination);