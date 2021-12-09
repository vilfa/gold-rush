#pragma once

#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include "Types/FWindow.h"

class Window
{
public:
    Window(const uint32_t _width,
           const uint32_t _height,
           const std::string _name,
           const int _gl_version_major = _GL_VERSION_MAJOR_,
           const int _gl_version_minor = _GL_VERSION_MINOR_,
           const bool _gl_use_multisampling = false,
           const int _gl_multisample_count = 2,
           int gl_profile = GLFW_OPENGL_CORE_PROFILE,
           GLFWmonitor *monitor = NULL,
           GLFWwindow *share = NULL);

    GLFWwindow *GetWindow() const;
    int GetWidth() const;
    void SetWidth(int width);
    int GetHeight() const;
    void SetHeight(int height);
    int GetWindowShouldClose() const;
    bool GetMultisamplingEnabled() const;
    void SetInputMode(int mode, int value);
    std::string GetWindowName() const;
    void SetWindowName(const std::string _name);
    void SetWindowShouldClose(bool should_close);
    void SetFramebufferSizeCallback(FramebufferSizeCallbackFunction callback);
    void SetMouseMoveCallback(MouseCallbackFunction callback);
    void SetMouseScrollCallback(MouseCallbackFunction callback);

private:
    GLFWwindow *window_;
    std::string name_;
    uint32_t width_;
    uint32_t height_;
    bool gl_use_multisampling_;
    uint32_t gl_multisample_count_;
    const int _gl_version_major_;
    const int _gl_version_minor_;
    int gl_profile_;

    static const int _GL_VERSION_MAJOR_;
    static const int _GL_VERSION_MINOR_;
};
