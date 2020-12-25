#include "Renderer/Renderer.h"

const glm::vec3 Renderer::_DEFAULT_CAMERA_POSITION_ = glm::vec3(0.0f, 15.0f, 0.0f);

Renderer::Renderer(Window& window,
	glm::vec3 camera_position) :
    window_(window),
	camera_(Camera(camera_position)),
    delta_time_(0.0),
    last_frame_(0.0),
    first_mouse_(true),
    last_x_((float)window.GetWidth() / 2.0f),
    last_y_((float)window.GetHeight() / 2.0f)
{
	setupInput(GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	setupGlobalEnables();
}

void Renderer::Render()
{
	UniformBuffer<glm::mat4> ubo_matrices(3, 0);
	UniformBuffer<glm::vec3> ubo_camera(1, 1);
	UniformBuffer<glm::vec3> ubo_light(1, 2);

	GameWorld world;

	while (!window_.GetWindowShouldClose())
	{
		processFrametime();
		setRenderStats();
		processKeyboardInput();
		clearFramebuffers();

		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = camera_.GetViewMatrix();
		glm::mat4 view_3 = camera_.GetViewMatrix3();
		glm::mat4 projection = camera_.GetProjectionMatrix();

		ubo_matrices.Data(projection, 0);
		ubo_matrices.Data(view, 1);
		ubo_matrices.Data(view_3, 2);
		ubo_camera.Data(camera_.position_, 0);
		ubo_light.Data(world.GetSunPosition(), 0);

		world.Draw();

		glfwSwapBuffers(window_.GetWindow());
		glfwPollEvents();
	}

	glfwTerminate();
}

void Renderer::FramebufferSizeCallback(GLFWwindow* window, int width,
	int height)
{
	glViewport(0, 0, width, height);
}

void Renderer::MouseMoveCallback(GLFWwindow* window, double x_pos,
	double y_pos)
{
	if (first_mouse_)
	{
		last_x_ = (float)x_pos;
		last_y_ = (float)y_pos;
		first_mouse_ = false;
	}

	float x_offset = (float)x_pos - last_x_;
	float y_offset = last_y_ - (float)y_pos; // Reversed since y-coordinates range from bottom to top.
	last_x_ = (float)x_pos;
	last_y_ = (float)y_pos;

	camera_.ProcessMouseMovement(x_offset, y_offset);
}

void Renderer::MouseScrollCallback(GLFWwindow* window, double x_offset,
	double y_offset)
{
	camera_.ProcessMouseScroll((float&)y_offset);
}

void Renderer::setupInput(int mode, int value)
{
	glfwSetInputMode(window_.GetWindow(), mode, value);
}

void Renderer::setupGlobalEnables()
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
	glEnable(GL_BLEND);
	glBlendEquation(GL_FUNC_ADD);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_FRAMEBUFFER_SRGB);
	
	if (window_.GetMultisamplingEnabled())
	{
		glEnable(GL_MULTISAMPLE);
	}
}

void Renderer::processKeyboardInput()
{
	if (glfwGetKey(window_.GetWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		window_.SetWindowShouldClose(true);
	}

	CAMSPDenum speed = (glfwGetKey(window_.GetWindow(), GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) 
		? CAMSPDenum::FAST : CAMSPDenum::NORMAL;

	if (glfwGetKey(window_.GetWindow(), GLFW_KEY_W) == GLFW_PRESS)
	{
		camera_.ProcessKeyboard(CAMMOVenum::FORWARD, speed, (float)delta_time_);
	}
	if (glfwGetKey(window_.GetWindow(), GLFW_KEY_S) == GLFW_PRESS)
	{
		camera_.ProcessKeyboard(CAMMOVenum::BACKWARD, speed, (float)delta_time_);
	}
	if (glfwGetKey(window_.GetWindow(), GLFW_KEY_A) == GLFW_PRESS)
	{
		camera_.ProcessKeyboard(CAMMOVenum::LEFT, speed, (float)delta_time_);
	}
	if (glfwGetKey(window_.GetWindow(), GLFW_KEY_D) == GLFW_PRESS)
	{
		camera_.ProcessKeyboard(CAMMOVenum::RIGHT, speed, (float)delta_time_);
	}
}

void Renderer::processFrametime()
{
	double now = glfwGetTime();
	delta_time_ = now - last_frame_;
	last_frame_ = now;
}

std::string Renderer::getRenderStats()
{
	return window_.GetWindowName() + 
		u8" • " + "FPS:" + 
		std::to_string(1 / delta_time_) +
		"|Frametime:" + 
		std::to_string(delta_time_ * 1000);
}

void Renderer::setRenderStats()
{
	window_.SetWindowName(getRenderStats());
}

void Renderer::clearFramebuffers()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
