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

    std::vector<Terrain::Vertex> vertices_;

    Terrain(const uint32_t _grid_size = 256, const float _height_scale = 10.0f);

    void Draw(Shader& shader);

    std::shared_ptr<std::vector<glm::vec3>> GetGrid();
    float GetHalfDimension();

    std::shared_ptr<std::vector<glm::mat4>> GetTree1ModelMats();
    std::shared_ptr<std::vector<glm::mat4>> GetTree2ModelMats();
    std::shared_ptr<std::vector<glm::mat4>> GetTree3ModelMats();
    std::shared_ptr<std::vector<glm::mat4>> GetBushModelMats();
    std::shared_ptr<std::vector<glm::mat4>> GetRockModelMats();
    std::shared_ptr<std::vector<glm::mat4>> GetGrassModelMats();

private:
    const uint32_t _grid_size_;
    const float _height_scale_;
    std::shared_ptr<std::vector<glm::vec3>> grid_;

    uint32_t vao_, vbo_;

    std::shared_ptr<std::vector<glm::mat4>> tree_1_model_mats_;
    std::shared_ptr<std::vector<glm::mat4>> tree_2_model_mats_;
    std::shared_ptr<std::vector<glm::mat4>> tree_3_model_mats_;
    std::shared_ptr<std::vector<glm::mat4>> bush_model_mats_;
    std::shared_ptr<std::vector<glm::mat4>> rock_model_mats_;
    std::shared_ptr<std::vector<glm::mat4>> grass_model_mats_;

    void setupVertices(std::vector<glm::vec3>& positions, std::vector<glm::vec3>& normals,
        std::vector<glm::vec3>& colors);
    void setupVegetation(std::vector<glm::vec3>& trees, std::vector<glm::vec3>& bushes,
        std::vector<glm::vec3>& rocks, std::vector<glm::vec3>& grass);
    void setupTerrain();
    glm::mat4 getPositionTransform();
    void scaleGridHeight();
};
