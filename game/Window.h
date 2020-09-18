#ifndef WINDOW_H
#define WINDOW_H

#include <iostream>
#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Window
{
public:
    Window(int width, int height, std::string windowName, 
        int glVersionMajor = _GL_VERSION_MAJOR, int glVersionMinor = _GL_VERSION_MINOR, 
        int glProfile = GLFW_OPENGL_CORE_PROFILE, GLFWmonitor* monitor = NULL, 
        GLFWwindow* share = NULL);

    GLFWwindow* getWindow() const;
    int getWidth() const;
    int getHeight() const;
    std::string getName() const;
    int getWindowShouldClose() const;
    void setWindowShouldClose(bool shouldClose);
    void setInputMode(int mode, int value);

private:
    GLFWwindow* window;
    int width;
    int height;
    std::string name;
    static const int _GL_VERSION_MAJOR;
    static const int _GL_VERSION_MINOR;
};
#endif // !WINDOW_H