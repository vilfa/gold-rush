#pragma once

#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Application/Window.h"
#include "Application/Callbacks.h"
#include "Renderer/Camera.h"
#include "Renderer/Shader.h"
#include "Renderer/Mesh.h"
#include "Renderer/Model.h"
#include "Renderer/Skybox.h"
#include "Terrain/Terrain.h"

class Renderer
{
public:
    Renderer(Window& window, glm::vec3 cameraPosition = _DEFAULT_CAMERA_POSITION);

    void Render();
    
    void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
    void MouseMoveCallback(GLFWwindow* window, double xPos, double yPos);
    void MouseScrollCallback(GLFWwindow* window, double xOffset, double yOffset);

private:
    Window& window;
    Camera camera;

    static const glm::vec3 _DEFAULT_CAMERA_POSITION;

    double deltaTime;
    double lastFrame;

    bool firstMouse;
    float lastX;
    float lastY;

    void setupInput(int mode, int value);
    void setupGlobalEnables();

    void processKeyboardInput();
};