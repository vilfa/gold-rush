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
#include "World/GWorld.h"

class Renderer
{
public:
    Renderer(
        Window& window, 
        glm::vec3 cameraPosition = _DEFAULT_CAMERA_POSITION
    );

    void Render();
    void FramebufferSizeCallback(
        GLFWwindow* window, 
        int width, 
        int height
    );
    void MouseMoveCallback(
        GLFWwindow* window, 
        double xPos, 
        double yPos
    );
    void MouseScrollCallback(
        GLFWwindow* window, 
        double xOffset, 
        double yOffset
    );

private:
    Window& window;
    Camera camera;
    double deltaTime;
    double lastFrame;
    bool firstMouse;
    float lastX;
    float lastY;

    static const glm::vec3 _DEFAULT_CAMERA_POSITION;

    void setupInput(int mode, int value);
    void setupGlobalEnables();
    void processKeyboardInput();
    void processFrametime();
    void clearFramebuffers();
    void setRenderStats();
    std::string getRenderStats();
};