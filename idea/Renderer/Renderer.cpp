#include "Renderer/Renderer.h"

Renderer::Renderer(Window& window) :
    window_(window),
    delta_time_(0.0),
    last_frame_(0.0),
    first_mouse_(true),
    last_x_((float)window.GetWidth() / 2.0f),
    last_y_((float)window.GetHeight() / 2.0f)
{
	setupInput(GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	setupGlobalEnables();
}

void Renderer::Render(Camera& camera, Player& player, GameWorld& world)
{
	UniformBuffer<glm::mat4> ubo_matrices(3, 0);
	UniformBuffer<glm::vec3> ubo_camera(1, 1);
	UniformBuffer<glm::vec3> ubo_light(1, 2);

	while (!window_.GetWindowShouldClose())
	{
		processFrametime();
		setRenderStats();
		ProcessKeyboard(camera, player, world);
		clearFramebuffers();

		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 view_3 = camera.GetViewMatrix3();
		glm::mat4 projection = camera.GetProjectionMatrix();

		ubo_matrices.Data(projection, 0);
		ubo_matrices.Data(view, 1);
		ubo_matrices.Data(view_3, 2);
		ubo_camera.Data(camera.position_, 0);
		ubo_light.Data(world.GetSunPosition(), 0);

		world.Draw();
		player.Draw();

		glfwSwapBuffers(window_.GetWindow());
		glfwPollEvents();
	}

	glfwTerminate();
}

void Renderer::ProcessFramebuffer(GLFWwindow* window, int width,
	int height)
{
	glViewport(0, 0, width, height);
}

void Renderer::ProcessMouse(GLFWwindow* window, Camera& camera,
	double x_pos, double y_pos)
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

	camera.HandleMouse(x_offset, y_offset);
}

void Renderer::ProcessKeyboard(Camera& camera, Player& player, GameWorld& world)
{
	if (glfwGetKey(window_.GetWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		window_.SetWindowShouldClose(true);
	}

	float velocity = player.movement_speed_ * delta_time_;

	if (glfwGetKey(window_.GetWindow(), GLFW_KEY_W) == GLFW_PRESS)
	{
		player.position_ += player.front_ * velocity;
		player.position_.y = world.GetGridHeight(player.position_);
		camera.FollowPlayer(player.GetPosition());
	}
	if (glfwGetKey(window_.GetWindow(), GLFW_KEY_S) == GLFW_PRESS)
	{
		player.position_ -= player.front_ * velocity;
		player.position_.y = world.GetGridHeight(player.position_);
		camera.FollowPlayer(player.GetPosition());
	}
	if (glfwGetKey(window_.GetWindow(), GLFW_KEY_A) == GLFW_PRESS)
	{
		player.position_ -= player.right_ * velocity;
		player.position_.y = world.GetGridHeight(player.position_);
		camera.FollowPlayer(player.GetPosition());
	}
	if (glfwGetKey(window_.GetWindow(), GLFW_KEY_D) == GLFW_PRESS)
	{
		player.position_ += player.right_ * velocity;
		player.position_.y = world.GetGridHeight(player.position_);
		camera.FollowPlayer(player.GetPosition());
	}

	std::cout << "RENDERER::PROCESS_KEYBOARD::CAMERA_POSITION" << std::endl;
	std::cout << "(" << camera.position_.x << "," << camera.position_.y << "," << camera.position_.z << ")" << std::endl;
	std::cout << "RENDERER::PROCESS_KEYBOARD::PLAYER_POSITION" << std::endl;
	std::cout << "(" << player.position_.x << "," << player.position_.y << "," << player.position_.z << ")" << std::endl;
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

void Renderer::processFrametime()
{
	double now = glfwGetTime();
	delta_time_ = now - last_frame_;
	last_frame_ = now;
}

void Renderer::clearFramebuffers()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

std::string Renderer::getRenderStats()
{
	return window_.GetWindowName() + " FPS:" + std::to_string(1 / delta_time_) + "|FT:" + std::to_string(delta_time_ * 1000);
}

void Renderer::setRenderStats()
{
	window_.SetWindowName(getRenderStats());
}