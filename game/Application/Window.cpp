#include "Application/Window.h"

const int Window::_GL_VERSION_MAJOR_ = 4;
const int Window::_GL_VERSION_MINOR_ = 2;

Window::Window(const uint32_t _width,
	const uint32_t _height,
	const std::string _name,
	const int _gl_version_major,
	const int _gl_version_minor,
	const bool _gl_use_multisampling,
	const int _gl_multisample_count,
	int gl_profile,
	GLFWmonitor* monitor,
	GLFWwindow* share) :
	width_(_width),
	height_(_height),
	name_(_name),
	_gl_version_major_(_gl_version_major),
	_gl_version_minor_(_gl_version_minor),
	gl_profile_(gl_profile),
	gl_use_multisampling_(_gl_use_multisampling),
	gl_multisample_count_(_gl_multisample_count)
{
	std::cout << "INFO::WINDOW::WINDOW::GLFW::INIT_START" << std::endl;
	std::cout << "INFO::WINDOW::WINDOW::OPEN_GL::VERSION" << std::endl;
	std::cout << "Using GL version major:" << _gl_version_major_ << std::endl;
	std::cout << "Using GL version minor:" << _gl_version_minor_ << std::endl;
	std::cout << "INFO::MAIN::GL_UNIFORM_BUFFER::GL_MAX_VERTEX_UNIFORM_COMPONENTS" << std::endl;
	std::cout << "Max uniform components:" << GL_MAX_VERTEX_UNIFORM_COMPONENTS << std::endl;
	std::cout << "INFO::WINDOW::WINDOW::OPEN_GL::ANTI_ALIASING" << std::endl;
	std::cout << "Using multisampling:" << gl_use_multisampling_ << std::endl;

    glfwInit();

	double time = glfwGetTime();
    
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, _gl_version_major_);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, _gl_version_minor_);
    glfwWindowHint(GLFW_OPENGL_PROFILE, gl_profile_);

	if (gl_use_multisampling_)
	{
		glfwWindowHint(GLFW_SAMPLES, gl_multisample_count_);
		std::cout << "Number of samples:" << gl_multisample_count_ << std::endl;
	}

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif // __APPLE__

	window_ = glfwCreateWindow(width_, height_, name_.c_str(), monitor, share);
	if (window_ == NULL)
	{
		std::cout << "ERROR::WINDOW::WINDOW::GLFW::INIT" << std::endl;
		glfwTerminate();
		std::exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window_);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "ERROR::WINDOW::WINDOW::GLAD::INIT" << std::endl;
		glfwTerminate();
		std::exit(EXIT_FAILURE);
	}

	std::cout << "INFO::WINDOW::WINDOW::INIT_IMGUI" << std::endl;

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	ImFontConfig font_cfg;
	font_cfg.SizePixels = 32.f;
	io.Fonts->AddFontDefault(&font_cfg);
	io.Fonts->Build();

	ImGui_ImplGlfw_InitForOpenGL(window_, true);
	ImGui_ImplOpenGL3_Init("#version 420 core");

	glViewport(0, 0, width_, height_);

	std::cout << "INFO::WINDOW::WINDOW::GLFW::INIT_END" << std::endl;
	std::cout << "Window init took:" << (glfwGetTime() - time) * 1000 << "ms" << std::endl;
}

GLFWwindow* Window::GetWindow() const
{
	return window_;
}

int Window::GetWidth() const
{
	return width_;
}

int Window::GetHeight() const
{
	return height_;
}

int Window::GetWindowShouldClose() const
{
	return glfwWindowShouldClose(window_);
}

std::string Window::GetWindowName() const
{
	return name_;
}

bool Window::GetMultisamplingEnabled() const
{
	return gl_use_multisampling_;
}

void Window::SetWindowShouldClose(bool shouldClose)
{
	glfwSetWindowShouldClose(window_, (GLboolean)shouldClose);
}

void Window::SetInputMode(int mode, int value)
{
	glfwSetInputMode(window_, mode, value);
}

void Window::SetWindowName(const std::string _name)
{
	glfwSetWindowTitle(window_, _name.c_str());
}

void Window::SetFramebufferSizeCallback(FramebufferSizeCallbackFunction callback)
{
	glfwSetFramebufferSizeCallback(window_, callback);
}

void Window::SetMouseMoveCallback(MouseCallbackFunction callback)
{
	glfwSetCursorPosCallback(window_, callback);
}

void Window::SetMouseScrollCallback(MouseCallbackFunction callback)
{
	glfwSetScrollCallback(window_, callback);
}
