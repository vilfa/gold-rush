#include "GWorld.h"

GWorld::GWorld() :
    gTerrain(Terrain(128)),
    gSkybox(Skybox("Resources/Skyboxes/Fantasy_01/", SKYBFORMATenum::PNG)),
    sTerrain(Shader("Resources/Shaders/Terrain/lowPolyTerrain.vert", "Resources/Shaders/Terrain/lowPolyTerrain.frag")),
    sSkybox(Shader("Resources/Shaders/Skybox/fantasySkybox.vert", "Resources/Shaders/Skybox/fantasySkybox.frag"))
{
}

void GWorld::Draw()
{
    drawTerrain();
    drawSkybox();
}

void GWorld::drawTerrain()
{
    gTerrain.Draw(sTerrain);
}

void GWorld::drawSkybox()
{
    gSkybox.Draw(sSkybox);
}
