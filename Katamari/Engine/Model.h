#pragma once

#include <fstream>
#include <vector>

#include "Math\Algebra.h"

class Model
{
public:
    Model(std::string path);

    std::vector<Vector3> vertices;
    std::vector<Vector3> normals;
    std::vector<Vector2> uvs;

private:
    bool LoadOBJ(std::string path,
                 std::vector<Vector3> & vertices,
                 std::vector<Vector3> & normals,
                 std::vector<Vector2> & uvs);
};