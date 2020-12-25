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
#include "World/TerrainElement.h"

class Renderer
{
public:
    Renderer(Window& window, 
        glm::vec3 camera_position = _DEFAULT_CAMERA_POSITION_);

    void Render();
    void FramebufferSizeCallback(GLFWwindow* window, int width, 
        int height);
    void MouseMoveCallback(GLFWwindow* window, double x_pos, 
        double y_pos);
    void MouseScrollCallback(GLFWwindow* window, double x_offset, 
        double y_offset);

private:
    Window& window_;
    Camera camera_;
    double delta_time_;
    double last_frame_;
    bool first_mouse_;
    float last_x_;
    float last_y_;

    static const glm::vec3 _DEFAULT_CAMERA_POSITION_;

    void setupInput(int mode, int value);
    void setupGlobalEnables();
    void processKeyboardInput();
    void processFrametime();
    void clearFramebuffers();
    void setRenderStats();
    std::string getRenderStats();
};