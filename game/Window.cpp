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

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "ERROR::INIT::GLAD" << std::endl;
		glfwTerminate();
		std::exit(EXIT_FAILURE);
	}

	glViewport(0, 0, this->width, this->height);
}

GLFWwindow* Window::getWindow() const
{
	return window;
}

int Window::getWidth() const
{
	return width;
}

int Window::getHeight() const
{
	return height;
}

std::string Window::getName() const
{
	return name;
}

int Window::getWindowShouldClose() const
{
	return glfwWindowShouldClose(window);
}

void Window::setWindowShouldClose(bool shouldClose)
{
	if (shouldClose)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
	else {
		glfwSetWindowShouldClose(window, GL_FALSE);
	}
}

void Window::setInputMode(int mode, int value)
{
	glfwSetInputMode(window, mode, value);
}
