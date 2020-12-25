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
#include "World/GQuadTree.h"
#include "Game/Player.h"
#include "Game/Prop.h"

class GWorld
{
public:
    std::vector<Prop> actors_;
    //Player player_;
    GQuadTree quad_tree_;

    GWorld();

    void Draw();

    std::shared_ptr<std::vector<glm::mat4>> GetTree1Mats();

private:
    Shader shader_terrain_, shader_skybox_;
    Skybox skybox_;
    Terrain terrain_;

    void drawTerrain();
    void drawSkybox();
};
