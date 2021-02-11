#pragma once

#include "Math\Algebra.h"

#include <fstream>
#include <unordered_map>
#include <vector>

bool LoadIndexedModel(std::string path,
    std::vector<Vector3> & positions,
    std::vector<Vector3> & normals,
    std::vector<Vector2> & uvs,
    std::vector<unsigned int>  & indices);

bool LoadOBJ(std::string path,
    std::vector<Vector3> & positions,
    std::vector<Vector3> & normals,
    std::vector<Vector2> & uvs);

bool IndexVBO(std::vector<Vector3> & in_positions,
    std::vector<Vector3> & in_normals,
    std::vector<Vector2> & in_uvs,
    std::vector<Vector3> & out_positions,
    std::vector<Vector3> & out_normals,
    std::vector<Vector2> & out_uvs,
    std::vector<unsigned int>  & out_indices);