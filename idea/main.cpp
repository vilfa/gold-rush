#include <iostream>
#include <random>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Window.h"
#include "Shader.h"
#include "Camera.h"
#include "Model.h"


void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window, Camera* camera, float deltaTime);
void mouseMoveCallback(GLFWwindow* window, double xPos, double yPos);
void mouseScrollCallback(GLFWwindow* window, double xOffset, double yOffset);

uint32_t loadTexture(const std::string& path);
uint32_t loadCubemap(const std::vector<std::string>& paths);

/*
* Window setup
*/
const uint32_t SCR_WIDTH = 1600;
const uint32_t SCR_HEIGHT = 900;
const std::string WINDOW_NAME = "big pp";

/*
* Camera setup
*/
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
double deltaTime = 0.0;
double lastFrame = 0.0;

/*
* Mouse setup
*/
bool firstMouse = true;
float lastX = SCR_WIDTH / 2;
float lastY = SCR_HEIGHT / 2;

/*
* Random
*/
std::random_device randomDevice;
std::mt19937 randomEngine(randomDevice());
std::uniform_real_distribution<float> randomDistribution(0.0f, 1.0f);

int main()
{
	/*----- WINDOW & GLOBAL ATTRIBUTES -----*/
	Window window(SCR_WIDTH, SCR_HEIGHT, WINDOW_NAME);
	window.SetFramebufferSizeCallback(framebufferSizeCallback);
	window.SetInputMode(GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	window.SetMouseMoveCallback(mouseMoveCallback);
	window.SetMouseScrollCallback(mouseScrollCallback);

	/*
	* Global enables. (glEnable function calls).
	* Use Window::SetGlobalEnable and Window::SetGlobalDisable instead of direct calls.
	* Makes the code more readable (will probably move this to a e.g. Renderer class).
	*/
	window.SetGlobalEnable(GL_DEPTH_TEST); // Enable depth buffer (depth testing). Don't forget to clear the buffer each frame.
	window.SetGlobalEnable(GL_BLEND); // Enable blending.
	window.SetGlobalEnable(GL_CULL_FACE); // Enable face culling.

	/*
	* Framebuffer, texture color buffer (the render output is stored to a texture, can easily be used in shaders) and
	* render buffer objects (can not be directly read from, which gives them a performance edge over using textures as
	* buffer objects).
	*/
	uint32_t framebuffer;
	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer); // Bind framebuffer.

	// The framebuffer must be complete (at least one buffer - color, depth or stencil; at least on color attachment;
	// all atachments should be complete - allocated memory; each buffer must have the same number of samples).

	uint32_t textureColorBuffer;
	glGenTextures(1, &textureColorBuffer);
	glBindTexture(GL_TEXTURE_2D, textureColorBuffer); // Bind texture color buffer.

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL); // Allocate memory - buffer completeness.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // Texture parameters don't really matter here.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // Texture parameters don't really matter here.

	glBindTexture(GL_TEXTURE_2D, 0); // Unbind the texture.

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorBuffer, 0); // Attach it to the currently bound framebuffer as a color buffer.


	// Render buffer object.
	uint32_t rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo); // Bind.

	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, SCR_WIDTH, SCR_HEIGHT); // Allocate memory.

	glBindRenderbuffer(GL_RENDERBUFFER, 0); // Unbind.

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); // Attach it to the currently bound framebuffer as a depth and stencil buffer.


	// Check that the frambuffer is complete.
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "ERROR::MAIN::FRAMEBUFFER::FRAMEBUFFER_INCOMPLETE" << std::endl;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0); // Unbind the framebuffer, we're not using it yet.
	

	/*----- GEOMETRY -----*/
	float skyboxVertices[] = {
		// positions          
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f
	};

	uint32_t skyboxVAO, skyboxVBO;
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (const void*)0);
	glBindVertexArray(0);

	float quadVertices[] = {
		// positions   // texCoords
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,
		 1.0f,  1.0f,  1.0f, 1.0f
	};

	uint32_t quadVAO, quadVBO;
	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &quadVBO);
	glBindVertexArray(quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (const void*) 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (const void*) (2 * sizeof(float)));
	glBindVertexArray(0);


	/*----- SHADERS -----*/
	Shader normalShader("resources/shaders/blending.vert", "resources/shaders/blending.frag");
	Shader backpackShader("resources/shaders/backpack_shader.vert", "resources/shaders/backpack_shader.frag");
	Shader framebufferShader("resources/shaders/framebuffer_shader.vert", "resources/shaders/framebuffer_shader.frag");
	Shader skyboxShader("resources/shaders/skybox_shader.vert", "resources/shaders/skybox_shader.frag");
	
	/*----- TEXTURES -----*/
	uint32_t cubeTexture = loadTexture("resources/textures/container2.png");
	uint32_t floorTexture = loadTexture("resources/textures/wall.jpg");
	uint32_t grassTexture = loadTexture("resources/textures/grass.png");
	uint32_t windowTexture = loadTexture("resources/textures/window.png");
	
	std::vector<std::string> skyboxPaths
	{
		"resources/skybox/right.jpg",
		"resources/skybox/left.jpg",
		"resources/skybox/top.jpg",
		"resources/skybox/bottom.jpg",
		"resources/skybox/front.jpg",
		"resources/skybox/back.jpg"
	};
	uint32_t skyboxTexture = loadCubemap(skyboxPaths);

	/*----- MODELS -----*/
	Model survivalBackpack("resources/models/backpack/backpack.obj");

	/*----- RENDER -----*/

	// Render parameters
	glBlendEquation(GL_FUNC_ADD); // This call can be omitted, since GL_FUNC_ADD is the default blend equation.
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glCullFace(GL_BACK); // Tell OpenGL which faces to cull. (default = GL_BACK)
	glFrontFace(GL_CCW); // The front faces are counter-clockwse faces. (default = GL_CCW)

	/*
	* So, to draw the scene to a single texture we'll have to take the following steps:
	*	Render the scene as usual with the new framebuffer bound as the active framebuffer.
	*	Bind to the default framebuffer.
	*	Draw a quad that spans the entire screen with the new framebuffer's color buffer as its texture.
	*/
	while (!window.GetWindowShouldClose())
	{
		/*--- Frame time logic ---*/
		double currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		window.SetWindowTitle(window.GetWindowTitle() + ("[fps:" + std::to_string(1 / deltaTime) + ",frametime:" + std::to_string(deltaTime * 1000) + "]"));

		/*--- Input ---*/
		processInput(window.GetWindow(), &camera, (float)deltaTime);

		/*--- Render commands ---*/
		//glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Not using the stencil buffer right now.
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

		/*--- Draw ---*/
		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view;
		glm::mat4 projection = glm::perspective(glm::radians(camera.Fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 200.0f);

		// Skybox
		skyboxShader.Use();
		view = glm::mat4(glm::mat3(camera.GetViewMatrix()));
		skyboxShader.SetMat4("view", view, GL_FALSE);
		skyboxShader.SetMat4("projection", projection, GL_FALSE);
		glDepthMask(GL_FALSE);
		glBindVertexArray(skyboxVAO);
		glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glDepthMask(GL_TRUE);

		// Backpack
		backpackShader.Use();
		view = camera.GetViewMatrix();
		backpackShader.SetMat4("view", view, GL_FALSE);
		backpackShader.SetMat4("projection", projection, GL_FALSE);
		backpackShader.SetMat4("model", model, GL_FALSE);
		survivalBackpack.Draw(backpackShader);

		/*--- Switch to default framebuffer and draw to window ---*/
		//glBindFramebuffer(GL_FRAMEBUFFER, 0); // Bind back to the default framebuffer.
		//glDisable(GL_DEPTH_TEST);

		//framebufferShader.Use();
		//glBindVertexArray(quadVAO);
		//glBindTexture(GL_TEXTURE_2D, textureColorBuffer);
		//glDrawArrays(GL_TRIANGLES, 0, 6);

		/*--- Events and buffers ---*/
		glfwSwapBuffers(window.GetWindow());
		glfwPollEvents();
	}

	/*
	* Clean up.
	*/
	glDeleteFramebuffers(1, &framebuffer);
	glDeleteVertexArrays(1, &quadVAO);
	glDeleteBuffers(1, &quadVBO);


	/*
	* Terminate program.
	*/
	glfwTerminate();
	return 0;
}

/*
* Handle window frame buffer size change.
*/
void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

/*
* Process the keyboard input.
*/
void processInput(GLFWwindow* window, Camera* camera, float deltaTime)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}

	CAMSPDenum speed;
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
	{
		speed = CAMSPDenum::FAST;
	}
	else
	{
		speed = CAMSPDenum::NORMAL;
	}

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		camera->ProcessKeyboard(CAMMOVenum::FORWARD, speed, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		camera->ProcessKeyboard(CAMMOVenum::BACKWARD, speed, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		camera->ProcessKeyboard(CAMMOVenum::LEFT, speed, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		camera->ProcessKeyboard(CAMMOVenum::RIGHT, speed, deltaTime);
	}
}

/*
* Process mouse movement input.
*/
void mouseMoveCallback(GLFWwindow* window, double xPos, double yPos)
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

/* 
* Process mouse scroll input.
*/
void mouseScrollCallback(GLFWwindow* window, double xOffset, double yOffset)
{
	camera.ProcessMouseScroll((float&)yOffset);
}

/*
* Utility texture loading function
*/
uint32_t loadTexture(const std::string& path)
{
	uint32_t textureID;
	glGenTextures(1, &textureID);

	int width, height, nComp;
	unsigned char* data = stbi_load(path.c_str(), &width, &height, &nComp, 0);
	if (data)
	{
		GLenum format = GL_RGB;
		if (nComp == 1)
		{
			format = GL_RED;
		}
		else if (nComp == 3)
		{
			format = GL_RGB;
		}
		else if (nComp == 4)
		{
			format = GL_RGBA;
		}

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		if (format == GL_RGBA)
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		}
		else
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		}

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "ERROR::MAIN::LOAD_TEXURE::TEXTURE_LOAD_ERROR" << std::endl;
		std::cout << "Path:" << path << std::endl;
		stbi_image_free(data);
	}
	
	return textureID;
}

/*
* Utility cubemap loading function.
*/
uint32_t loadCubemap(const std::vector<std::string>& paths)
{
	uint32_t textureID;
	glGenTextures(1, &textureID);

	int width, height, nComp;
	unsigned char* data;
	for (std::size_t i = 0; i < paths.size(); i++)
	{
		data = stbi_load(paths[i].c_str(), &width, &height, &nComp, 0);
		if (data)
		{
			glTexImage2D(
				GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
			);
			
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

			stbi_image_free(data);
		}
		else
		{
			std::cout << "ERROR::MAIN::LOAD_CUBEMAP::CUBEMAP_LOAD_ERROR" << std::endl;
			std::cout << "Cubemap load failed at path: " << paths[i] << std::endl;
			stbi_image_free(data);
		}
	}

	return textureID;
}