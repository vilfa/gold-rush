#pragma once

#include <iostream>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <Renderer/Shader.h>
#include <Terrain/TerrainGenerator.h>

class TerrainGenerator; // Forward declaration.

/*
* This class is basically a simpler Mesh class. The reason I did this is because
* the Mesh class is a bit too complicated for this terrain. This is definitely not
* an ideal solution and is something that should be addressed in the future - the 
* entire Renderer API requires a somewhat higher level of abstaction. So yeah I guess
* I'm leaving that to the future me, I bet he'll be super happy.
*/
class Terrain
{
public:
    struct Vertex
    {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec3 color;
    };

    std::vector<Terrain::Vertex> Vertices;
    
    Terrain(const int gridSize);
    void Draw(Shader& shader);

private:
    const int gridSize;
    uint32_t VAO, VBO;

    void setupVertices(std::vector<glm::vec3>& positions, std::vector<glm::vec3>& normals, std::vector<glm::vec3>& colors);
    void setupTerrain();
};

