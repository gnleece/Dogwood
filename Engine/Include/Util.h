#pragma once

#define GLEW_STATIC
#include <GL/glew.h>
#include <tinyxml2.h>
#include <string>

#include "Math\Algebra.h"
#include "Rendering\Color.h"

using std::string;

unsigned int            MakeGuid(string str);

Vector3                 ReadVector3FromXML(tinyxml2::XMLElement* xmlnode);
tinyxml2::XMLElement*   WriteVector3ToXML(Vector3 vector, string name, tinyxml2::XMLDocument& doc);

ColorRGB                ReadColorFromXML(tinyxml2::XMLElement* xmlnode);
tinyxml2::XMLElement*   WriteColorToXML(ColorRGB Color, string name, tinyxml2::XMLDocument& doc);

tinyxml2::XMLElement*   WriteIntToXML(int value, string nodeName, string attribName, tinyxml2::XMLDocument& doc);
tinyxml2::XMLElement*   WriteUnsignedIntToXML(unsigned int value, string nodeName, string attribName, tinyxml2::XMLDocument& doc);
tinyxml2::XMLElement*   WriteFloatToXML(float value, string nodeName, string attribName, tinyxml2::XMLDocument& doc);
tinyxml2::XMLElement*   WriteStringToXML(string str, string nodeName, string attribName, tinyxml2::XMLDocument& doc);

Vector3                 ReadVector3FromString(string str);
string                  WriteVector3ToString(Vector3 str);

void                    FileCopy(string source, string destination);

string                  GetFriendlyAssetNameFromPath(string path);

float                   RandomFloat(float min, float max);