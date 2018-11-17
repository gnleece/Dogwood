#pragma once

#include <string>

#include "Math\Algebra.h"
#include "Rendering\Color.h"

using std::string;

unsigned int            MakeGuid(string str);

Vector3                 ReadVector3FromString(string str);
string                  WriteVector3ToString(Vector3 str);

void                    FileCopy(string source, string destination);

string                  GetFriendlyAssetNameFromPath(string path);

template <typename T> void Swap(T& a, T& b)
{
    T temp = a;
    a = b;
    b = temp;
}

template <typename T> bool PComp(const T * const & a, const T * const & b)
{
    return *a < *b;
}