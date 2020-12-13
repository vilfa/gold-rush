#include "Application/Window.h"

const int Window::_GL_VERSION_MAJOR = 3;
const int Window::_GL_VERSION_MINOR = 3;

Window::Window(const uint32_t& width, const uint32_t& height, const std::string& windowName, 
	const int glVersionMajor, const int glVersionMinor,
	const bool glUseMultisampling, const int glNumberOfSamples,
	int glProfile, GLFWmonitor* monitor, GLFWwindow* share) :
	width(width),
	height(height),
	name(windowName)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, glVersionMajor);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, glVersionMinor);
    glfwWindowHint(GLFW_OPENGL_PROFILE, glProfile);

	std::cout << "INFO::WINDOW::WINDOW::OPEN_GL::VERSION" << std::endl;
	std::cout << "Using GL version major:" << glVersionMajor << std::endl;
	std::cout << "Using GL version minor:" << glVersionMinor << std::endl;
	std::cout << "INFO::WINDOW::WINDOW::OPEN_GL::ANTI_ALIASING" << std::endl;
	std::cout << "Using multisampling:" << glUseMultisampling << std::endl;

	if (glUseMultisampling)
	{
		glfwWindowHint(GLFW_SAMPLES, glNumberOfSamples);
		std::cout << "Number of samples:" << glNumberOfSamples << std::endl;
	}

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif // __APPLE__

	window = glfwCreateWindow(this->width, this->height, this->name.c_str(), monitor, share);
	if (window == NULL)
	{
		std::cout << "ERROR::WINDOW::WINDOW::GLFW::INIT" << std::endl;
		glfwTerminate();
		std::exit(EXIT_FAILURE);
	}

	glfwSetWindowUserPointer(window, this);
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "ERROR::WINDOW::WINDOW::GLAD::INIT" << std::endl;
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

std::string Window::GetWindowTitle() const
{
	return name;
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

void Window::SetWindowTitle(const std::string& title)
{
	glfwSetWindowTitle(window, title.c_str());
}

void Window::SetGlobalEnable(GLenum global)
{
	glEnable(global);
}

void Window::SetGlobalDisable(GLenum global)
{
	glDisable(global);
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
