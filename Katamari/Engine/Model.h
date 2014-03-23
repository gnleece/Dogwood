#pragma once

#include <fstream>
#include <unordered_map>
#include <vector>

#include "Math\Algebra.h"
#include "Rendering\Vertex.h"

#define GLEW_STATIC
#include <GL/glew.h>


// TODO this is really just a Mesh so rename it to that

class Model
{
public:
    Model(std::string path);

    std::vector<Vector3> positions;
    std::vector<Vector3> normals;
    std::vector<Vector2> uvs;
    std::vector<GLuint>  indices;

private:

    bool LoadOBJ(std::string path,
                 std::vector<Vector3> & vertices,
                 std::vector<Vector3> & normals,
                 std::vector<Vector2> & uvs);

    bool IndexVBO(std::vector<Vector3> & in_vertices,
                  std::vector<Vector3> & in_normals,
                  std::vector<Vector2> & in_uvs,
                  std::vector<Vector3> & out_vertices,
                  std::vector<Vector3> & out_normals,
                  std::vector<Vector2> & out_uvs,
                  std::vector<GLuint>  & out_indices);

    bool FindIndex(Vertex & vertex, std::unordered_map<Vertex, GLuint> & map, GLuint &index);
};