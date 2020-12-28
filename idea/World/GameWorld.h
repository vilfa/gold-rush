#pragma once

#include <iostream>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/hash.hpp>
#include <unordered_map>
#include <utility>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Terrain/Terrain.h"
#include "Renderer/Shader.h"
#include "Renderer/Skybox.h"
#include "Renderer/Camera.h"
#include "World/GObject.h"
#include "World/QuadTree.h"
#include "World/TerrainElement.h"
#include "Game/Player.h"
#include "Game/Entity.h"

typedef std::vector<std::shared_ptr<std::vector<glm::mat4>>> ModelMatrixVector;

class GameWorld
{
public:
    QuadTree quad_tree_;
    std::vector<Entity> game_entities_;

    GameWorld(glm::vec3 sun_position = glm::vec3(0.0f, -1.0f, 0.0f), uint32_t grid_size_ = 128);

    void Draw();

    float GetGridHeight(glm::vec3 player_pos);
    glm::vec3& GetSunPosition();
    void SetSunPosition(glm::vec3 new_sun_pos);
    void RemoveCollectibles(std::vector<Entity> collectible, Player& player);

private:
    const uint32_t _grid_size_;
    std::shared_ptr<std::vector<glm::vec3>> grid_;

    Shader shader_terrain_, shader_skybox_, shader_entity_;
    Skybox skybox_;
    Terrain terrain_;
    TerrainElement trrel_tree_1_, trrel_tree_2_, trrel_tree_3_, 
        trrel_bush_, trrel_rock_, trrel_grass_, trrel_hazelnut_;

    ModelMatrixVector model_mats_all_;
    glm::vec3 sun_position_;

    std::vector<std::pair<glm::mat4, int>> hazelnut_model_mats_pairs_;
    std::unordered_map<glm::mat4, int, std::hash<glm::mat4>> hazelnut_index_map_;

    void setupModelMatsAll();
    void createGameEntities();
    void createQuadTree();
    void createModelMatPairs();
    void createIndexMap();
    void drawTerrain();
    void drawSkybox();
    void drawWoodland();
};
