#include "Renderer/Renderer.h"

const glm::vec3 Renderer::_DEFAULT_CAMERA_POSITION = glm::vec3(10.0f, 15.0f, 10.0f);

Renderer::Renderer(Window& window, glm::vec3 cameraPosition) :
    window(window),
	camera(Camera(window, cameraPosition)),
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
	/*----- UNIFORM BLOCKS ------*/
	uint32_t uboMatrices, uboCamera, uboWorldLight;
	glGenBuffers(1, &uboMatrices);
	glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices); // The target is GL_UNIFORM_BUFFER.
	glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW); // Allocate for view and projection matrix.
	glBindBuffer(GL_UNIFORM_BUFFER, 0); // Unbind.

	glGenBuffers(1, &uboCamera);
	glBindBuffer(GL_UNIFORM_BUFFER, uboCamera);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::vec3), NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	glGenBuffers(1, &uboWorldLight);
	glBindBuffer(GL_UNIFORM_BUFFER, uboWorldLight);
	glBufferData(GL_UNIFORM_BUFFER, 4 * sizeof(glm::vec3), NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	glBindBufferRange(GL_UNIFORM_BUFFER, 0, uboMatrices, 0, 2 * sizeof(glm::mat4)); // Bind the entire buffer to binding point 0.
	glBindBufferRange(GL_UNIFORM_BUFFER, 1, uboCamera, 0, sizeof(glm::vec3));
	glBindBufferRange(GL_UNIFORM_BUFFER, 2, uboWorldLight, 0, 4 * sizeof(glm::vec3));

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	/*----- RESOURCES -----*/

	std::vector<glm::vec3> light = {
		glm::vec3(-1.0f, -1.0f, -1.0f),
		glm::vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(0.2f, 0.2f, 0.2f),
		glm::vec3(0.4f, 0.4f, 0.4f),
	};

	Shader terrainShader("Resources/Shaders/Terrain/lowPolyTerrain.vert", "Resources/Shaders/Terrain/lowPolyTerrain.frag");
	Shader treeShader("Resources/Shaders/Model/lowPolyTree.vert", "Resources/Shaders/Model/lowPolyTree.frag");

	Shader skyboxShader("Resources/Shaders/Skybox/fantasySkybox.vert", "Resources/Shaders/Skybox/fantasySkybox.frag");
	Skybox skyboxFantasy("Resources/Skyboxes/Fantasy_01/", SKYBFORMATenum::PNG);

	Shader normalVisualiser("Resources/Shaders/.old/backpack_with_normals.vert",
		"Resources/Shaders/.old/backpack_with_normals.geom",
		"Resources/Shaders/.old/backpack_with_normals.frag");

	Terrain lowPolyTerrain(512);

	Model tree1("Resources/Models/tree_1/tree_1.obj", true);
	//Model tree2("Resources/Models/tree_2/tree_2.obj", true);
	//Model tree3("Resources/Models/tree_3/tree_3.obj", true);
	//Model treeTrunk("Resources/Models/tree_trunk/tree_trunk.obj", true);

	std::shared_ptr<std::vector<glm::mat4>> treeInstanceModelMats = lowPolyTerrain.GetTreeModelMats();

	while (!window.GetWindowShouldClose())
	{
		processFrametime();
		setRenderStats();
		processKeyboardInput();
		clearFramebuffers();

		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 projection = camera.GetProjectionMatrix();

		glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(projection));
		glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(view));
		glBindBuffer(GL_UNIFORM_BUFFER, 0);

		glBindBuffer(GL_UNIFORM_BUFFER, uboCamera);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::vec3), glm::value_ptr(camera.Position));
		glBindBuffer(GL_UNIFORM_BUFFER, 0);

		glBindBuffer(GL_UNIFORM_BUFFER, uboWorldLight);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, 4 * sizeof(glm::vec3), light.data());
		glBindBuffer(GL_UNIFORM_BUFFER, 0);

		terrainShader.Use();
		terrainShader.SetMat4("model", model);
		lowPolyTerrain.Draw(terrainShader);
		
		/*normalVisualiser.Use();
		normalVisualiser.SetMat4("model", model);
		lowPolyTerrain.Draw(normalVisualiser);*/

		treeShader.Use();
		tree1.DrawInstanced(treeShader, treeInstanceModelMats);

		/*treeShader.SetMat4("model", model);
		tree1.Draw(treeShader);

		model = glm::translate(model, glm::vec3(0.0f, 10.0f, 0.0f));
		treeShader.SetMat4("model", model);
		tree2.Draw(treeShader);
		
		model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
		treeShader.SetMat4("model", model);
		tree3.Draw(treeShader);

		model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
		treeShader.SetMat4("model", model);
		treeTrunk.Draw(treeShader);*/

		// Skybox
		skyboxShader.Use();
		view = glm::mat4(glm::mat3(camera.GetViewMatrix()));
		skyboxShader.SetMat4("projection", projection);
		skyboxShader.SetMat4("view", view);
		skyboxFantasy.Draw(skyboxShader);

		/*--- Events and buffers ---*/
		glfwSwapBuffers(window.GetWindow());
		glfwPollEvents();
	}

	glfwTerminate();
}

void Renderer::FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void Renderer::MouseMoveCallback(GLFWwindow* window, double xPos, double yPos)
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

void Renderer::MouseScrollCallback(GLFWwindow* window, double xOffset, double yOffset)
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
	glCullFace(GL_BACK); // Tell OpenGL which faces to cull. (default = GL_BACK)
	glFrontFace(GL_CCW); // The front faces are counter-clockwse faces. (default = GL_CCW)

	glEnable(GL_BLEND);
	glBlendEquation(GL_FUNC_ADD); // This call can be omitted, since GL_FUNC_ADD is the default blend equation.
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_FRAMEBUFFER_SRGB); // Enable gamma correction. On most displays this means applying the exponent 1/2.2 to a fragment output color. This is called sRGB color space.
	
	if (window.GetMultisamplingEnabled())
	{
		glEnable(GL_MULTISAMPLE); // Enable anti-aliasing. The technique used is MSAA (Multi Sample Anti-Aliasing).
	}
}

void Renderer::processKeyboardInput()
{
	if (glfwGetKey(window.GetWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		window.SetWindowShouldClose(true);
	}

	CAMSPDenum speed;
	if (glfwGetKey(window.GetWindow(), GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
	{
		speed = CAMSPDenum::FAST;
	}
	else
	{
		speed = CAMSPDenum::NORMAL;
	}

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
	double currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;
}

std::string Renderer::getRenderStats()
{
	return window.GetWindowTitle() + (" FPS:" + std::to_string(1 / deltaTime) + "|Frametime:" + std::to_string(deltaTime * 1000));
}

void Renderer::setRenderStats()
{
	window.SetWindowTitle(getRenderStats());
}

void Renderer::clearFramebuffers()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
