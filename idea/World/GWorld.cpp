#include "GWorld.h"

GWorld::GWorld() :
    terrain_(Terrain(128)),
    skybox_(Skybox("Resources/Skyboxes/Fantasy_01/", SKYBFORMATenum::PNG)),
    shader_terrain_(Shader("Resources/Shaders/Terrain/lowPolyTerrain.vert", "Resources/Shaders/Terrain/lowPolyTerrain.frag")),
    shader_skybox_(Shader("Resources/Shaders/Skybox/fantasySkybox.vert", "Resources/Shaders/Skybox/fantasySkybox.frag"))
{
}

void GWorld::Draw()
{
    drawTerrain();
    drawSkybox();
}

std::shared_ptr<std::vector<glm::mat4>> GWorld::GetTree1Mats()
{
    return terrain_.GetTree1ModelMats();
}

void GWorld::drawTerrain()
{
    terrain_.Draw(shader_terrain_);
}

void GWorld::drawSkybox()
{
    skybox_.Draw(shader_skybox_);
}
