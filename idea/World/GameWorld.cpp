#include "GameWorld.h"

GameWorld::GameWorld(glm::vec3 sun_position) :
    terrain_(Terrain(128)),
    skybox_(Skybox("Resources/Skyboxes/Fantasy_01/", SKYBFORMATenum::PNG)),
    quad_tree_(AABB(glm::vec3(0.0f), terrain_.GetHalfDimension())),
    shader_terrain_(Shader("Resources/Shaders/Terrain/lowPolyTerrain.vert", "Resources/Shaders/Terrain/lowPolyTerrain.frag")),
    shader_skybox_(Shader("Resources/Shaders/Skybox/fantasySkybox.vert", "Resources/Shaders/Skybox/fantasySkybox.frag")),
    shader_entity_(Shader("Resources/Shaders/Model/lowPolyTerrainElement.vert", "Resources/Shaders/Model/lowPolyTerrainElement.frag")),
    trrel_tree_1_(Model("Resources/Models/tree_1/tree_1.obj", true), shader_entity_),
    trrel_tree_2_(Model("Resources/Models/tree_2/tree_2.obj", true), shader_entity_),
    trrel_tree_3_(Model("Resources/Models/tree_3/tree_3.obj", true), shader_entity_),
    trrel_bush_(Model("Resources/Models/lil_bush/lil_bush.obj", true), shader_entity_),
    trrel_rock_(Model("Resources/Models/rock/rock.obj", true), shader_entity_),
    trrel_grass_(Model("Resources/Models/grass_bud/grass_bud.obj", true), shader_entity_),
    sun_position_(sun_position)
{
    setupModelMatsAll();
    createGameEntities();
}

void GameWorld::Draw()
{
    drawTerrain();
    drawSkybox();
    drawWoodland();
}

void GameWorld::setupModelMatsAll()
{
    model_mats_all_.push_back(terrain_.GetTree1ModelMats());
    model_mats_all_.push_back(terrain_.GetTree2ModelMats());
    model_mats_all_.push_back(terrain_.GetTree3ModelMats());
    model_mats_all_.push_back(terrain_.GetBushModelMats());
    model_mats_all_.push_back(terrain_.GetRockModelMats());
    model_mats_all_.push_back(terrain_.GetGrassModelMats());
}

glm::vec3& GameWorld::GetSunPosition()
{
    return sun_position_;
}

void GameWorld::SetSunPosition(glm::vec3 new_sun_pos)
{
    sun_position_ = new_sun_pos;
}

void GameWorld::createGameEntities()
{
    for (std::size_t i = 0; i < model_mats_all_.at(0)->size(); i++)
    {
        game_entities_.push_back(Entity(trrel_tree_1_, model_mats_all_.at(0)->at(i)));
    }
    for (std::size_t i = 0; i < model_mats_all_.at(1)->size(); i++)
    {
        game_entities_.push_back(Entity(trrel_tree_2_, model_mats_all_.at(1)->at(i)));
    }
    for (std::size_t i = 0; i < model_mats_all_.at(2)->size(); i++)
    {
        game_entities_.push_back(Entity(trrel_tree_3_, model_mats_all_.at(2)->at(i)));
    }
    for (std::size_t i = 0; i < model_mats_all_.at(3)->size(); i++)
    {
        game_entities_.push_back(Entity(trrel_bush_, model_mats_all_.at(3)->at(i)));
    }
    for (std::size_t i = 0; i < model_mats_all_.at(4)->size(); i++)
    {
        game_entities_.push_back(Entity(trrel_rock_, model_mats_all_.at(4)->at(i)));
    }
    for (std::size_t i = 0; i < model_mats_all_.at(5)->size(); i++)
    {
        game_entities_.push_back(Entity(trrel_grass_, model_mats_all_.at(5)->at(i)));
    }
}

void GameWorld::drawTerrain()
{
    terrain_.Draw(shader_terrain_);
}

void GameWorld::drawSkybox()
{
    skybox_.Draw(shader_skybox_);
}

void GameWorld::drawWoodland()
{
    trrel_tree_1_.DrawInstanced(model_mats_all_.at(0));
    trrel_tree_2_.DrawInstanced(model_mats_all_.at(1));
    trrel_tree_3_.DrawInstanced(model_mats_all_.at(2));
    trrel_bush_.DrawInstanced(model_mats_all_.at(3));
    trrel_rock_.DrawInstanced(model_mats_all_.at(4));
    trrel_grass_.DrawInstanced(model_mats_all_.at(5));
}
