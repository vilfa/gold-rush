#include "Game.h"

const glm::vec3 Game::_DEFAULT_CAMERA_POSITION_ = glm::vec3(0.0f, 0.0f, 0.0f);
const glm::vec3 Game::_DEFAULT_PLAYER_POSITION_ = glm::vec3(0.0f, 0.0f, 0.0f);
const glm::vec3 Game::_WORLD_CENTER_ = glm::vec3(0.0f, 0.0f, 0.0f);

Game::Game(Window& window) :
    renderer_(window),
    camera_(Camera(_DEFAULT_CAMERA_POSITION_)),
    game_world_(GameWorld()),
    player_(Player(_DEFAULT_PLAYER_POSITION_))
{
    glm::vec3 player_start_pos = _DEFAULT_PLAYER_POSITION_;
    player_start_pos.y = game_world_.GetGridHeight(player_start_pos);
    player_.position_ = player_start_pos;
    camera_.FollowPlayer(player_start_pos);
}

void Game::Start()
{
    renderer_.Render(camera_, player_, game_world_);
}

void Game::Update()
{
}

void Game::HandleFramebuffer(GLFWwindow* window, int width,
    int height)
{
    renderer_.ProcessFramebuffer(window, width, height);
}

void Game::HandleMouse(GLFWwindow* window, double x_pos,
    double y_pos)
{
    renderer_.ProcessMouse(window, camera_, x_pos, y_pos);
}
