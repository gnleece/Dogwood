#pragma once

#define GLEW_STATIC
#include <GL/glew.h>
#include <tinyxml2.h>

#include "Math\Algebra.h"
#include "Rendering\Colour.h"

using namespace tinyxml2;

Vector3 ReadVector3FromXML(XMLElement* xmlnode);
ColourRGB ReadColourFromXML(XMLElement* xmlnode);