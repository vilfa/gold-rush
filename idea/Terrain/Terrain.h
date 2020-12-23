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

    std::vector<Terrain::Vertex> Vertices;
    
    Terrain(const uint32_t gridSize);
    
    void Draw(Shader& shader);
    std::shared_ptr<std::vector<glm::vec3>> GetTrees1();
    std::shared_ptr<std::vector<glm::vec3>> GetTrees2();
    std::shared_ptr<std::vector<glm::vec3>> GetTrees3();
    std::shared_ptr<std::vector<glm::vec3>> GetBushes();
    std::shared_ptr<std::vector<glm::vec3>> GetRocks();
    std::shared_ptr<std::vector<glm::vec3>> GetGrass();

    std::shared_ptr<std::vector<glm::mat4>> GetTree1ModelMats();
    std::shared_ptr<std::vector<glm::mat4>> GetTree2ModelMats();
    std::shared_ptr<std::vector<glm::mat4>> GetTree3ModelMats();
    std::shared_ptr<std::vector<glm::mat4>> GetBushModelMats();
    std::shared_ptr<std::vector<glm::mat4>> GetRockModelMats();
    std::shared_ptr<std::vector<glm::mat4>> GetGrassModelMats();

private:
    const uint32_t gridSize;
    uint32_t VAO, VBO;

    std::shared_ptr<std::vector<glm::vec3>> tree1Positions;
    std::shared_ptr<std::vector<glm::vec3>> tree2Positions;
    std::shared_ptr<std::vector<glm::vec3>> tree3Positions;
    std::shared_ptr<std::vector<glm::vec3>> bushPositions;
    std::shared_ptr<std::vector<glm::vec3>> rockPositions;
    std::shared_ptr<std::vector<glm::vec3>> grassPositions;

    std::shared_ptr<std::vector<glm::mat4>> tree1ModelMats;
    std::shared_ptr<std::vector<glm::mat4>> tree2ModelMats;
    std::shared_ptr<std::vector<glm::mat4>> tree3ModelMats;
    std::shared_ptr<std::vector<glm::mat4>> bushModelMats;
    std::shared_ptr<std::vector<glm::mat4>> rockModelMats;
    std::shared_ptr<std::vector<glm::mat4>> grassModelMats;

    void setupVertices(
        std::vector<glm::vec3>& positions, 
        std::vector<glm::vec3>& normals, 
        std::vector<glm::vec3>& colors
    );
    void setupVegetation(
        std::vector<glm::vec3>& trees,
        std::vector<glm::vec3>& bushes,
        std::vector<glm::vec3>& rocks,
        std::vector<glm::vec3>& grass
    );
    void setupTerrain();
    glm::mat4 getTransform();
};

