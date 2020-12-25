#pragma once

#include <iostream>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Terrain/Terrain.h"
#include "Renderer/Shader.h"
#include "Renderer/Skybox.h"
#include "Renderer/Camera.h"
#include "World/GObject.h"
#include "World/QuadTree.h"
#include "Game/Player.h"
#include "World/TerrainElement.h"
#include "Game/Entity.h"

typedef std::vector<std::shared_ptr<std::vector<glm::mat4>>> ModelMatrixVector;

class GameWorld
{
public:
    QuadTree quad_tree_;

    std::vector<Entity> game_entities_;
    //Player player_;

    GameWorld(glm::vec3 sun_position = glm::vec3(0.0f, -1.0f, 0.0f));

    void Draw();

    glm::vec3& GetSunPosition();
    void SetSunPosition(glm::vec3 new_sun_pos);

private:
    Shader shader_terrain_, shader_skybox_, shader_entity_;
    Skybox skybox_;
    Terrain terrain_;
    TerrainElement trrel_tree_1_, trrel_tree_2_, trrel_tree_3_, trrel_bush_, trrel_rock_, trrel_grass_;

    ModelMatrixVector model_mats_all_;
    glm::vec3 sun_position_;

    void setupModelMatsAll();
    void createGameEntities();
    void drawTerrain();
    void drawSkybox();
    void drawWoodland();
};
