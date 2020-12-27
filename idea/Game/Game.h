#pragma once

#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Application/Window.h"
#include "Renderer/Renderer.h"
#include "Renderer/Camera.h"
#include "World/GameWorld.h"
#include "Game/Player.h"

class Game
{
public:
    Game(Window& window);

    void Start();

    void Update();

    void HandleFramebuffer(GLFWwindow* window, int width,
        int height);
    void HandleMouse(GLFWwindow* window, double x_pos,
        double y_pos);

private:
    Renderer renderer_;
    Camera camera_;
    GameWorld game_world_;
    Player player_;

    static const glm::vec3 _DEFAULT_CAMERA_POSITION_;
    static const glm::vec3 _DEFAULT_PLAYER_POSITION_;
    static const glm::vec3 _WORLD_CENTER_;
};
