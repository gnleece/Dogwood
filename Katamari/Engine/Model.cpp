#include "Model.h"

Model::Model(std::string path)
{
    bool success = LoadOBJ(path, vertices, normals, uvs);
}

// Based on OBJ loading tutorial from: http://www.opengl-tutorial.org/beginners-tutorials/tutorial-7-model-loading/
bool Model::LoadOBJ(std::string path,
    std::vector<Vector3> & vertices,
    std::vector<Vector3> & normals,
    std::vector<Vector2> & uvs)
{
    FILE * file;
    errno_t err = fopen_s(&file, path.c_str(), "r");
    if (err)
    {
        printf("Could not open model file: %s\n", path.c_str());
        return false;
    }
    
    std::vector< unsigned int > vertexIndices, normalIndices, uvIndices;
    std::vector<Vector3> temp_vertices;
    std::vector<Vector3> temp_normals;
    std::vector<Vector2> temp_uvs;

    while (true)
    {
        char lineHeader[128];
        // read the first word of the line
        int res = fscanf_s(file, "%s", lineHeader, _countof(lineHeader));
        if (res == EOF)
            break;

        // else : parse lineHeader
        if (strcmp(lineHeader, "v") == 0)           // vertex
        {
            Vector3 vertex;
            fscanf_s(file, "%f %f %f\n", vertex.Start(), vertex.Start()+1, vertex.Start()+2);
            temp_vertices.push_back(vertex);
        }
        else if (strcmp(lineHeader, "vn") == 0)     // normal
        {
            Vector3 normal;
            fscanf_s(file, "%f %f %f\n", normal.Start(), normal.Start()+1, normal.Start()+2);
            temp_normals.push_back(normal);
        }
        else if (strcmp(lineHeader, "vt") == 0)     // uv
        {
            Vector2 uv;
            fscanf_s(file, "%f %f\n", uv.Start(), uv.Start()+1);
            temp_uvs.push_back(uv);
        }
        else if (strcmp(lineHeader, "f") == 0)      // face
        {
            std::string vertex1, vertex2, vertex3;
            unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
            int matches = fscanf_s(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
            if (matches != 9)
            {
                printf("File can't be read by our simple parser : ( Try exporting with other options\n");
                return false;
            }
            vertexIndices.push_back(vertexIndex[0]);
            vertexIndices.push_back(vertexIndex[1]);
            vertexIndices.push_back(vertexIndex[2]);
            normalIndices.push_back(normalIndex[0]);
            normalIndices.push_back(normalIndex[1]);
            normalIndices.push_back(normalIndex[2]);
            uvIndices.push_back(uvIndex[0]);
            uvIndices.push_back(uvIndex[1]);
            uvIndices.push_back(uvIndex[2]);
        }
    }

    // For each vertex of each triangle
    for (unsigned int i = 0; i < vertexIndices.size(); i++)
    {
        unsigned int vertexIndex = vertexIndices[i];
        Vector3 vertex = temp_vertices[vertexIndex - 1];
        vertices.push_back(vertex);
    }
    for (unsigned int i = 0; i < normalIndices.size(); i++)
    {
        unsigned int normalIndex = normalIndices[i];
        Vector3 normal = temp_normals[normalIndex - 1];
        normals.push_back(normal);
    }
    for (unsigned int i = 0; i < uvIndices.size(); i++)
    {
        unsigned int uvIndex = uvIndices[i];
        Vector2 uv = temp_uvs[uvIndex - 1];
        uvs.push_back(uv);
    }

    fclose(file);

    return true;
}