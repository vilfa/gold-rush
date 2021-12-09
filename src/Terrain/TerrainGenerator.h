#pragma once

#include <algorithm>
#include <iostream>
#include <iterator>
#include <random>
#include <string>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Terrain/NoiseGenerator.h"

class TerrainGenerator
{
public:
    TerrainGenerator(const uint32_t _grid_size = 256);

    std::shared_ptr<std::vector<glm::vec3>> GetGrid();

    std::vector<glm::vec3> &GetPositions();
    std::vector<glm::vec3> &GetNormals();
    std::vector<glm::vec3> &GetColors();

    std::vector<glm::vec3> &GetTrees();
    std::vector<glm::vec3> &GetBushes();
    std::vector<glm::vec3> &GetRocks();
    std::vector<glm::vec3> &GetGrass();
    std::vector<glm::vec3> &GetHazelnuts();

private:
    const uint32_t _grid_size_;
    std::shared_ptr<float[]> height_map_;
    std::shared_ptr<std::vector<glm::vec3>> grid_;

    std::vector<glm::vec3> positions_;
    std::vector<glm::vec3> normals_;
    std::vector<glm::vec3> colors_;

    std::vector<glm::vec3> tree_positions_;
    std::vector<glm::vec3> bush_positions_;
    std::vector<glm::vec3> rock_positions_;
    std::vector<glm::vec3> grass_positions_;
    std::vector<glm::vec3> hazelnut_positions_;

    void generateHeightMap();
    void generateGrid();
    void generateVertexPositions();
    void generateVertexColors();
    void generateVegetationPositions();
    glm::vec3 calculateTriangleNormal(glm::vec3 v0, glm::vec3 v1,
                                      glm::vec3 v2);
};
