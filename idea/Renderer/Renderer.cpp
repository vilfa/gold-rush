#include "Renderer/Renderer.h"

const glm::vec3 Renderer::_DEFAULT_CAMERA_POSITION = glm::vec3(0.0f, 15.0f, 0.0f);

Renderer::Renderer(
	Window& window, 
	glm::vec3 cameraPosition
) :
    window(window),
	camera(Camera(cameraPosition)),
    deltaTime(0.0),
    lastFrame(0.0),
    firstMouse(true),
    lastX((float)window.GetWidth() / 2.0f),
    lastY((float)window.GetHeight() / 2.0f)
{
	setupInput(GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	setupGlobalEnables();
}

void Renderer::Render()
{
	UniformBuffer<glm::mat4> uboMatrices(3, 0);
	UniformBuffer<glm::vec3> uboCamera(1, 1);
	UniformBuffer<glm::vec3> uboLight(1, 2);

	/*----- RESOURCES -----*/
	Shader woodlandShader("Resources/Shaders/Model/lowPolyWoodland.vert", "Resources/Shaders/Model/lowPolyWoodland.frag");
	Shader normalVisShader("Resources/Shaders/Terrain/terrainNormals.vert", "Resources/Shaders/Terrain/terrainNormals.geom", "Resources/Shaders/Terrain/terrainNormals.frag");
	
	Model tree1("Resources/Models/tree_1/tree_1.obj", true);
	Model tree2("Resources/Models/tree_2/tree_2.obj", true);
	Model tree3("Resources/Models/tree_3/tree_3.obj", true);
	Model bush("Resources/Models/lil_bush/lil_bush.obj", true);
	Model rock("Resources/Models/rock/rock.obj", true);
	Model grass("Resources/Models/grass_bud/grass_bud.obj", true);
	
	GWorld world;
	glm::vec3 sun(0.0f, -1.0f, 0.0f);

	/*std::shared_ptr<std::vector<glm::mat4>> tree1ImMats = lowPolyTerrain.GetTree1ModelMats();
	std::shared_ptr<std::vector<glm::mat4>> tree2ImMats = lowPolyTerrain.GetTree2ModelMats();
	std::shared_ptr<std::vector<glm::mat4>> tree3ImMats = lowPolyTerrain.GetTree3ModelMats();
	std::shared_ptr<std::vector<glm::mat4>> bushesImMats = lowPolyTerrain.GetBushModelMats();
	std::shared_ptr<std::vector<glm::mat4>> rocksImMats = lowPolyTerrain.GetRockModelMats();
	std::shared_ptr<std::vector<glm::mat4>> grassImMats = lowPolyTerrain.GetGrassModelMats();*/

	while (!window.GetWindowShouldClose())
	{
		processFrametime();
		setRenderStats();
		processKeyboardInput();
		clearFramebuffers();

		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 view3 = camera.GetViewMatrix3();
		glm::mat4 projection = camera.GetProjectionMatrix();

		uboMatrices.Data(projection, 0);
		uboMatrices.Data(view, 1);
		uboMatrices.Data(view3, 2);
		uboCamera.Data(camera.Position, 0);
		uboLight.Data(sun, 0);

		world.Draw();

		/*woodlandShader.Use();
		tree1.DrawInstanced(woodlandShader, tree1ImMats);
		tree2.DrawInstanced(woodlandShader, tree2ImMats);
		tree3.DrawInstanced(woodlandShader, tree3ImMats);
		bush.DrawInstanced(woodlandShader, bushesImMats);
		rock.DrawInstanced(woodlandShader, rocksImMats);
		grass.DrawInstanced(woodlandShader, grassImMats);*/

		/*--- Events and buffers ---*/
		glfwSwapBuffers(window.GetWindow());
		glfwPollEvents();
	}

	glfwTerminate();
}

void Renderer::FramebufferSizeCallback(
	GLFWwindow* window, 
	int width, 
	int height
)
{
	glViewport(0, 0, width, height);
}

void Renderer::MouseMoveCallback(
	GLFWwindow* window, 
	double xPos, 
	double yPos
)
{
	if (firstMouse)
	{
		lastX = (float)xPos;
		lastY = (float)yPos;
		firstMouse = false;
	}

	float xOffset = (float)xPos - lastX;
	float yOffset = lastY - (float)yPos; // Reversed since y-coordinates range from bottom to top.
	lastX = (float)xPos;
	lastY = (float)yPos;

	camera.ProcessMouseMovement(xOffset, yOffset);
}

void Renderer::MouseScrollCallback(
	GLFWwindow* window, 
	double xOffset, 
	double yOffset
)
{
	camera.ProcessMouseScroll((float&)yOffset);
}

void Renderer::setupInput(int mode, int value)
{
	glfwSetInputMode(window.GetWindow(), mode, value);
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
	
	if (window.GetMultisamplingEnabled())
	{
		glEnable(GL_MULTISAMPLE);
	}
}

void Renderer::processKeyboardInput()
{
	if (glfwGetKey(window.GetWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		window.SetWindowShouldClose(true);
	}

	CAMSPDenum speed = (glfwGetKey(window.GetWindow(), GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) 
		? CAMSPDenum::FAST : CAMSPDenum::NORMAL;

	if (glfwGetKey(window.GetWindow(), GLFW_KEY_W) == GLFW_PRESS)
	{
		camera.ProcessKeyboard(CAMMOVenum::FORWARD, speed, (float)deltaTime);
	}
	if (glfwGetKey(window.GetWindow(), GLFW_KEY_S) == GLFW_PRESS)
	{
		camera.ProcessKeyboard(CAMMOVenum::BACKWARD, speed, (float)deltaTime);
	}
	if (glfwGetKey(window.GetWindow(), GLFW_KEY_A) == GLFW_PRESS)
	{
		camera.ProcessKeyboard(CAMMOVenum::LEFT, speed, (float)deltaTime);
	}
	if (glfwGetKey(window.GetWindow(), GLFW_KEY_D) == GLFW_PRESS)
	{
		camera.ProcessKeyboard(CAMMOVenum::RIGHT, speed, (float)deltaTime);
	}
}

void Renderer::processFrametime()
{
	double now = glfwGetTime();
	deltaTime = now - lastFrame;
	lastFrame = now;
}

std::string Renderer::getRenderStats()
{
	return window.GetWindowTitle() + 
		u8" • " + "FPS:" + 
		std::to_string(1 / deltaTime) + 
		"|Frametime:" + 
		std::to_string(deltaTime * 1000);
}

void Renderer::setRenderStats()
{
	window.SetWindowTitle(getRenderStats());
}

void Renderer::clearFramebuffers()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
