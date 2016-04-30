#pragma once

#define GLEW_STATIC
#include <GL/glew.h>
#include <string>

#include "Math\Algebra.h"
#include "Rendering\Color.h"

using std::string;

unsigned int            MakeGuid(string str);

Vector3                 ReadVector3FromString(string str);
string                  WriteVector3ToString(Vector3 str);

void                    FileCopy(string source, string destination);

string                  GetFriendlyAssetNameFromPath(string path);