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

class GWorld
{
public:
    std::vector<GObject> Actors;
    GQuadTree QuadTree;

    GWorld();

    void Draw();

private:
    Shader sTerrain, sSkybox;
    Skybox gSkybox;
    Terrain gTerrain;

    
    void drawTerrain();
    void drawSkybox();
};
