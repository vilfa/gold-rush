#include "GameWorld.h"

GameWorld::GameWorld(glm::vec3 sun_position, uint32_t grid_size_) :
    _grid_size_(grid_size_),
    terrain_(Terrain(grid_size_)),
    skybox_(Skybox("Resources/Skyboxes/Fantasy_01/", SKYBFORMATenum::PNG)),
    quad_tree_(AABB(glm::vec3(0.0f), (float)grid_size_)),
    shader_terrain_(Shader("Resources/Shaders/Terrain/lowPolyTerrain.vert", "Resources/Shaders/Terrain/lowPolyTerrain.frag")),
    shader_skybox_(Shader("Resources/Shaders/Skybox/fantasySkybox.vert", "Resources/Shaders/Skybox/fantasySkybox.frag")),
    shader_entity_(Shader("Resources/Shaders/Model/lowPolyModel.vert", "Resources/Shaders/Model/lowPolyModel.frag")),
    trrel_tree_1_(Model("Resources/Models/tree_1/tree_1.obj", true), shader_entity_),
    trrel_tree_2_(Model("Resources/Models/tree_2/tree_2.obj", true), shader_entity_),
    trrel_tree_3_(Model("Resources/Models/tree_3/tree_3.obj", true), shader_entity_),
    trrel_bush_(Model("Resources/Models/lil_bush/lil_bush.obj", true), shader_entity_),
    trrel_rock_(Model("Resources/Models/rock/rock.obj", true), shader_entity_),
    trrel_grass_(Model("Resources/Models/grass_bud/grass_bud.obj", true), shader_entity_),
    sun_position_(sun_position)
{
    grid_ = terrain_.GetGrid();
    setupModelMatsAll();
    createGameEntities();
    createQuadTree();
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

float GameWorld::GetGridHeight(glm::vec3 player_pos)
{
    int grid_center, i, j;
    // Starting grid width height is (128, 128)
    // Generate map of (128, 128) and scale positions by 2, map still (128, 128), but corrdinates from [0, 256]
    // Translate map by grid width in -x and -z directions, so coordiantes are now [-128, 128]
    // Center of height map is at index (64, 64) since it's scaled * 2 and translated
    grid_center = _grid_size_ / 2; 
    j = grid_center + (player_pos.z / 2);
    i = grid_center + (player_pos.x / 2);

    std::cout << "GAME_WORLD::GET_GRID_HEIGHT::TERRAIN_HEIGHT_INDEX" << std::endl;
    std::cout << "(" << i << "," << j << ")" << std::endl;

    float p0, p1, p2, p3;
    p0 = grid_->at((i - 1) * _grid_size_ + j).y;
    p1 = grid_->at(i * _grid_size_ + (j + 1)).y;
    p2 = grid_->at(i * _grid_size_ + (j - 1)).y;
    p3 = grid_->at((i + 1) * _grid_size_ + j).y;

    return (p0 + p1 + p2 + p3) / 4.0f;
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

void GameWorld::createQuadTree()
{
    for (std::size_t i = 0; i < game_entities_.size(); i++)
    {
        quad_tree_.Insert(game_entities_.at(i));
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
