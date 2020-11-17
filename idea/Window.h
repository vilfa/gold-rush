#ifndef WINDOW_H
#define WINDOW_H

#include <iostream>
#include <string>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

typedef void (*FramebufferSizeCallbackFunction)(GLFWwindow*, int, int);
typedef void (*MouseCallbackFunction)(GLFWwindow*, double, double);

class Window
{
public:
    Window(const uint32_t& width, const uint32_t& height, const std::string& windowName, 
        int glVersionMajor = _GL_VERSION_MAJOR, int glVersionMinor = _GL_VERSION_MINOR, 
        int glProfile = GLFW_OPENGL_CORE_PROFILE, GLFWmonitor* monitor = NULL, 
        GLFWwindow* share = NULL);

    GLFWwindow* GetWindow() const;
    std::string GetName() const;
    int GetWidth() const;
    int GetHeight() const;
    int GetWindowShouldClose() const;
    std::string GetWindowTitle() const;
    void SetWindowShouldClose(bool shouldClose);
    void SetInputMode(int mode, int value);
    void SetWindowTitle(const std::string& title);
    void SetGlobalEnable(GLenum global);
    void SetGlobalDisable(GLenum global);
    void SetFramebufferSizeCallback(FramebufferSizeCallbackFunction callback);
    void SetMouseMoveCallback(MouseCallbackFunction callback);
    void SetMouseScrollCallback(MouseCallbackFunction callback);

private:
    GLFWwindow* window;
    std::string name;
    uint32_t width;
    uint32_t height;
    static const int _GL_VERSION_MAJOR;
    static const int _GL_VERSION_MINOR;
};
#endif // !WINDOW_H