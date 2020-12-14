#pragma once

#include<iostream>
#include<vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Renderer/Renderer.h"
#include "Terrain/NoiseGenerator.h"

class TerrainGenerator
{
public:
    TerrainGenerator(const int gridSize = 256);

    Mesh GenerateTerrain();

private:
    NoiseGenerator ng;
    int gridSize;

    float* heightMap;

    std::vector<glm::vec3> positions;
    std::vector<glm::vec3> normals;
    std::vector<uint32_t> indices;

    void generateHeightMap();
    std::vector<glm::vec3> generateGrid();

    void constructVertexPositions();
    glm::vec3 calculateTriangleNormals(glm::vec3 v0, glm::vec3 v1, glm::vec3 v2);
};

