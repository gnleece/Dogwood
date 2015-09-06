#include "Rendering\ModelLoading.h"

#include "Rendering\Vertex.h"

bool FindIndex(Vertex & vertex, std::unordered_map<Vertex, GLuint> & map, GLuint &index);

bool LoadIndexedModel(std::string path, 
    std::vector<Vector3> & positions,
    std::vector<Vector3> & normals,
    std::vector<Vector2> & uvs,
    std::vector<GLuint>  & indices)
{
    // TODO check if file extension is .obj

    std::vector<Vector3> tempPositions;
    std::vector<Vector3> tempNormals;
    std::vector<Vector2> tempUVs;
    bool success = LoadOBJ(path, tempPositions, tempNormals, tempUVs);
    int originalVertexCount = tempPositions.size();
    if (success)
    {
        IndexVBO(tempPositions, tempNormals, tempUVs, positions, normals, uvs, indices);
        printf("* Successfully loaded model: %s\n", path.c_str());
        printf("* Indexing reduced vertex count from %d to %d\n", originalVertexCount, positions.size());
    }
    return success;
}


// Based on OBJ loading tutorial from: http://www.opengl-tutorial.org/beginners-tutorials/tutorial-7-model-loading/
bool LoadOBJ(std::string path,
    std::vector<Vector3> & positions,
    std::vector<Vector3> & normals,
    std::vector<Vector2> & uvs)
{
    FILE * file;
    errno_t err = fopen_s(&file, path.c_str(), "r");
    if (err)
    {
        printf("Could not open mesh file: %s\n", path.c_str());
        return false;
    }

    std::vector< unsigned int > positionsIndices, normalIndices, uvIndices;
    std::vector<Vector3> tempPositions;
    std::vector<Vector3> tempNormals;
    std::vector<Vector2> tempUVs;

    bool hasUVs = false;

    while (true)
    {
        char lineHeader[128];
        // read the first word of the line
        int res = fscanf_s(file, "%s", lineHeader, _countof(lineHeader));
        if (res == EOF)
            break;

        // else : parse lineHeader
        if (strcmp(lineHeader, "v") == 0)           // position
        {
            Vector3 position;
            fscanf_s(file, "%f %f %f\n", position.Start(), position.Start() + 1, position.Start() + 2);
            tempPositions.push_back(position);
        }
        else if (strcmp(lineHeader, "vn") == 0)     // normal
        {
            Vector3 normal;
            fscanf_s(file, "%f %f %f\n", normal.Start(), normal.Start() + 1, normal.Start() + 2);
            tempNormals.push_back(normal);
        }
        else if (strcmp(lineHeader, "vt") == 0)     // uv
        {
            Vector2 uv;
            fscanf_s(file, "%f %f\n", uv.Start(), uv.Start() + 1);
            tempUVs.push_back(uv);
            hasUVs = true;
        }
        else if (strcmp(lineHeader, "f") == 0)      // face
        {
            if (hasUVs)
            {
                std::string vertex1, vertex2, vertex3;
                unsigned int positionIndex[3], uvIndex[3], normalIndex[3];
                int matches = fscanf_s(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &positionIndex[0], &uvIndex[0], &normalIndex[0], &positionIndex[1], &uvIndex[1], &normalIndex[1], &positionIndex[2], &uvIndex[2], &normalIndex[2]);
                if (matches != 9)
                {
                    printf("File can't be read by parser. Check README for format requirements.\n");
                    return false;
                }
                positionsIndices.push_back(positionIndex[0]);
                positionsIndices.push_back(positionIndex[1]);
                positionsIndices.push_back(positionIndex[2]);
                normalIndices.push_back(normalIndex[0]);
                normalIndices.push_back(normalIndex[1]);
                normalIndices.push_back(normalIndex[2]);
                uvIndices.push_back(uvIndex[0]);
                uvIndices.push_back(uvIndex[1]);
                uvIndices.push_back(uvIndex[2]);
            }
            else
            {
                std::string vertex1, vertex2, vertex3;
                unsigned int positionIndex[3], normalIndex[3];
                int matches = fscanf_s(file, "%d//%d %d//%d %d//%d\n", &positionIndex[0], &normalIndex[0], &positionIndex[1], &normalIndex[1], &positionIndex[2], &normalIndex[2]);
                if (matches != 6)
                {
                    printf("File can't be read by parser. Check README for format requirements.\n");
                    return false;
                }
                positionsIndices.push_back(positionIndex[0]);
                positionsIndices.push_back(positionIndex[1]);
                positionsIndices.push_back(positionIndex[2]);
                normalIndices.push_back(normalIndex[0]);
                normalIndices.push_back(normalIndex[1]);
                normalIndices.push_back(normalIndex[2]);
            }
        }
    }

    for (unsigned int i = 0; i < positionsIndices.size(); i++)
    {
        unsigned int positionIndex = positionsIndices[i];
        Vector3 position = tempPositions[positionIndex - 1];
        positions.push_back(position);
    }
    for (unsigned int i = 0; i < normalIndices.size(); i++)
    {
        unsigned int normalIndex = normalIndices[i];
        Vector3 normal = tempNormals[normalIndex - 1];
        normals.push_back(normal);
    }
    if (hasUVs)
    {
        for (unsigned int i = 0; i < uvIndices.size(); i++)
        {
            unsigned int uvIndex = uvIndices[i];
            Vector2 uv = tempUVs[uvIndex - 1];
            uvs.push_back(uv);
        }
    }

    fclose(file);

    return true;
}

bool IndexVBO(std::vector<Vector3> & in_positions,
    std::vector<Vector3> & in_normals,
    std::vector<Vector2> & in_uvs,
    std::vector<Vector3> & out_positions,
    std::vector<Vector3> & out_normals,
    std::vector<Vector2> & out_uvs,
    std::vector<GLuint>  & out_indices)
{
    std::unordered_map<Vertex, GLuint> vertexToIndexMap;
    for (unsigned int i = 0; i < in_positions.size(); i++)
    {
        GLuint index;
        Vector2 uv = i < in_uvs.size() ? in_uvs[i] : Vector2::Zero;
        Vertex vertex(in_positions[i], in_normals[i], uv);
        bool found = FindIndex(vertex, vertexToIndexMap, index);
        if (found)
        {
            // vertex already existed, yay! index it
            out_indices.push_back(index);
        }
        else
        {
            // vertex doesn't already exist, so add it
            out_positions.push_back(in_positions[i]);
            out_normals.push_back(in_normals[i]);
            if (i < in_uvs.size())
            {
                out_uvs.push_back(in_uvs[i]);
            }

            int newIndex = out_positions.size() - 1;
            out_indices.push_back(newIndex);
            vertexToIndexMap[vertex] = newIndex;
        }
    }

    return true;
}

bool FindIndex(Vertex & vertex, std::unordered_map<Vertex, GLuint> & map, GLuint & index)
{
    std::unordered_map<Vertex, GLuint>::iterator it = map.find(vertex);
    if (it != map.end())
    {
        index = it->second;
        return true;
    }
    return false;
}