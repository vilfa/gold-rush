#pragma once

#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Application/Window.h"
#include "Buffers/UniformBuffer.h"
#include "Renderer/Camera.h"
#include "Renderer/Shader.h"
#include "Renderer/Mesh.h"
#include "Renderer/Model.h"
#include "World/GameWorld.h"
#include "Game/Player.h"
#include "World/TerrainElement.h"

class Renderer
{
public:
    Renderer(Window& window);

    void Render(Camera& camera, Player& player, GameWorld& world);

    void ProcessFramebuffer(GLFWwindow* window, int width, 
        int height);
    void ProcessMouse(Camera& camera, Player& player, GLFWwindow* window,
        double x_pos, double y_pos);
    void ProcessKeyboard(Camera& camera, Player& player, GameWorld& world);

private:
    Window& window_;
    double delta_time_;
    double last_frame_;
    bool first_mouse_;
    float last_x_;
    float last_y_;

    void setupInput(int mode, int value);
    void setupGlobalEnables();
    void processFrametime();
    void clearFramebuffers();
    void setRenderStats();
    std::string getRenderStats();
};