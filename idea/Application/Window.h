#pragma once

#include <iostream>
#include <string>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Application/Callbacks.h"

class Window
{
public:
    Window(
        const uint32_t& width, 
        const uint32_t& height, 
        const std::string& windowName, 
        const int glVersionMajor = _GL_VERSION_MAJOR, 
        const int glVersionMinor = _GL_VERSION_MINOR,
        const bool glUseMultisampling = false, 
        const int glNumberOfSamples = 2,
        int glProfile = GLFW_OPENGL_CORE_PROFILE, 
        GLFWmonitor* monitor = NULL, 
        GLFWwindow* share = NULL
    );

    GLFWwindow* GetWindow() const;
    std::string GetWindowTitle() const;
    int GetWidth() const;
    int GetHeight() const;
    int GetWindowShouldClose() const;
    bool GetMultisamplingEnabled() const;
    
    void SetInputMode(int mode, int value);
    void SetWindowTitle(const std::string& title);
    void SetWindowShouldClose(bool shouldClose);

    void SetFramebufferSizeCallback(FramebufferSizeCallbackFunction callback);
    void SetMouseMoveCallback(MouseCallbackFunction callback);
    void SetMouseScrollCallback(MouseCallbackFunction callback);

private:
    GLFWwindow* window;
    std::string name;
    uint32_t width;
    uint32_t height;

    const bool glUseMultisampling;
    const int glMultisampleCount;

    static const int _GL_VERSION_MAJOR;
    static const int _GL_VERSION_MINOR;
};