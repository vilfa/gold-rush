#include "Window.h"

const int Window::_GL_VERSION_MAJOR = 3;
const int Window::_GL_VERSION_MINOR = 3;

Window::Window(int width, int height, std::string windowName, int glVersionMajor, int glVersionMinor, int glProfile, 
	GLFWmonitor* monitor, GLFWwindow* share)
{
	this->width = width;
	this->height = height;
	this->name = windowName;

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, glVersionMajor);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, glVersionMinor);
    glfwWindowHint(GLFW_OPENGL_PROFILE, glProfile);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif // __APPLE__

	window = glfwCreateWindow(this->width, this->height, this->name.c_str(), monitor, share);
	if (window == NULL)
	{
		std::cout << "ERROR::INIT::GLFW::WINDOW" << std::endl;
		glfwTerminate();
		std::exit(EXIT_FAILURE);
	}

	glfwSetWindowUserPointer(window, this);
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "ERROR::INIT::GLAD" << std::endl;
		glfwTerminate();
		std::exit(EXIT_FAILURE);
	}

	glViewport(0, 0, this->width, this->height);
}

GLFWwindow* Window::GetWindow() const
{
	return window;
}

std::string Window::GetName() const
{
	return name;
}

int Window::GetWidth() const
{
	return width;
}

int Window::GetHeight() const
{
	return height;
}

int Window::GetWindowShouldClose() const
{
	return glfwWindowShouldClose(window);
}

void Window::SetWindowShouldClose(bool shouldClose)
{
	if (shouldClose)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
	else {
		glfwSetWindowShouldClose(window, GL_FALSE);
	}
}

void Window::SetInputMode(int mode, int value)
{
	glfwSetInputMode(window, mode, value);
}

void Window::SetFramebufferSizeCallback(FramebufferSizeCallbackFunction callback)
{
	glfwSetFramebufferSizeCallback(window, callback);
}

void Window::SetMouseMoveCallback(MouseCallbackFunction callback)
{
	glfwSetCursorPosCallback(window, callback);
}

void Window::SetMouseScrollCallback(MouseCallbackFunction callback)
{
	glfwSetScrollCallback(window, callback);
}
