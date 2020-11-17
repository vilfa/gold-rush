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

/*
* Window setup
*/
const uint32_t SCR_WIDTH = 1280;
const uint32_t SCR_HEIGHT = 720;
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
	* Enable depth buffer (depth testing). The buffer must be cleared for each frame
	* so we don't store the information of the previous frame.
	*/
	window.SetGlobalEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_ALWAYS);
	//glDepthFunc(GL_LESS);
	//glDepthMask(GL_FALSE); // Do you want to make the depth buffer read-only? | setting this value to 1 has no real effect

	/*
	* Enable stencil testing (used for outlinining objects)
	*/
	window.SetGlobalEnable(GL_STENCIL_TEST);
	// sets what actions we perform based on the acquired stencil buffer
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE); // function signature: glStencilOp(GLenum sfail, GLenum dpfail, GLenum dppass)

	/*----- GEOMETRY -----*/
	/*glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	glm::vec3 pointLightPositions[] = {
		glm::vec3(0.7f,  0.2f,  2.0f),
		glm::vec3(2.3f, -3.3f, -4.0f),
		glm::vec3(-4.0f,  2.0f, -12.0f),
		glm::vec3(0.0f,  0.0f, -3.0f)
	};*/

	float cubeVertices[] = {
		// positions          // texture Coords
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};
	float planeVertices[] = {
		// positions          // texture Coords (note we set these higher than 1 (together with GL_REPEAT as texture wrapping mode). this will cause the floor texture to repeat)
		 5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
		-5.0f, -0.5f,  5.0f,  0.0f, 0.0f,
		-5.0f, -0.5f, -5.0f,  0.0f, 2.0f,

		 5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
		-5.0f, -0.5f, -5.0f,  0.0f, 2.0f,
		 5.0f, -0.5f, -5.0f,  2.0f, 2.0f
	};
	// cube VAO
	unsigned int cubeVAO, cubeVBO;
	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &cubeVBO);
	glBindVertexArray(cubeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glBindVertexArray(0);
	// plane VAO
	unsigned int planeVAO, planeVBO;
	glGenVertexArrays(1, &planeVAO);
	glGenBuffers(1, &planeVBO);
	glBindVertexArray(planeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), &planeVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glBindVertexArray(0);

	
	/*----- SHADERS -----*/
	//Shader objectShader("resources/shaders/model_loading.vert", "resources/shaders/model_loading.frag");
	Shader depthTestingShader("resources/shaders/depth_testing.vert", "resources/shaders/depth_testing.frag");
	Shader normalShader("resources/shaders/stencils.vert", "resources/shaders/stencils.frag");
	Shader singleColorShader("resources/shaders/single_color.vert", "resources/shaders/single_color.frag");
	
	/*----- TEXTURES -----*/
	uint32_t cubeTexture = loadTexture("resources/textures/container2.png");
	uint32_t floorTexture = loadTexture("resources/textures/wall.jpg");

	/*----- MODELS -----*/
	//Model survivalBackpack("resources/models/backpack/backpack.obj");

	//depthTestingShader.Use();
	//depthTestingShader.SetInt("texture1", 0);
	normalShader.Use();
	normalShader.SetInt("texture1", 0);

	/*----- RENDER -----*/
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
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear color and depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); // Clear color and depth buffer
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

		/*--- Draw ---*/
		////objectShader.Use();
		//depthTestingShader.Use();

		//// View and projection transformations
		//glm::mat4 projection = glm::perspective(glm::radians(camera.Fov), (float)window.GetWidth() / (float)window.GetHeight(), 0.1f, 200.0f);
		////objectShader.SetMat4("projection", projection, false);
		//depthTestingShader.SetMat4("projection", projection, false);
		//glm::mat4 view = camera.GetViewMatrix();
		////objectShader.SetMat4("view", view, false);
		//depthTestingShader.SetMat4("view", view, false);

		//// World transformations
		//glm::mat4 model = glm::mat4(1.0f);
		//
		//model = glm::mat4(1.0f);
		//model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		////objectShader.SetMat4("model", model, false);
		//depthTestingShader.SetMat4("model", model, false);

		////survivalBackpack.Draw(objectShader);


		//depthTestingShader.Use();
		normalShader.Use();

		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 projection = glm::perspective(glm::radians(camera.Fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 200.0f);

		normalShader.SetMat4("view", view, GL_FALSE);
		normalShader.SetMat4("projection", projection, GL_FALSE);

		// TURN OFF STENCIL TESTING
		glStencilMask(0x00); // DISABLE writing to stencil buffer, first draw floor

		// DRAW FLOOR
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, floorTexture);
		
		glBindVertexArray(planeVAO);

		normalShader.SetMat4("model", model, GL_FALSE);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);

		// TURN ON STENCIL TESTING
		glStencilFunc(GL_ALWAYS, 1, 0xFF); // function signature: glStencilFunc(GLenum func, GLint ref, GLuint mask) | ALL FRAGMENTS SHOULD PASS STENCIL TEST
		glStencilMask(0xFF); // ENABLE writing to stencil buffer, write bits to stencil buffer as is

		// DRAW CUBES
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, cubeTexture);

		glBindVertexArray(cubeVAO);

		model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
		normalShader.SetMat4("model", model, GL_FALSE);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
		normalShader.SetMat4("model", model, GL_FALSE);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		
		// TURN OFF STENCIL TESTING
		glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
		glStencilMask(0x00);
		glDisable(GL_DEPTH_TEST);

		// DRAW CUBE OUTLINES
		singleColorShader.Use();
		view = camera.GetViewMatrix();
		projection = glm::perspective(glm::radians(camera.Fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 200.0f);
		singleColorShader.SetMat4("view", view, GL_FALSE);
		singleColorShader.SetMat4("projection", projection, GL_FALSE);

		model = glm::mat4(1.0f);
		glBindVertexArray(cubeVAO);
		model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
		model = glm::scale(model, glm::vec3(1.15f, 1.15f, 1.15f));
		singleColorShader.SetMat4("model", model, GL_FALSE);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.15f, 1.15f, 1.15f));
		singleColorShader.SetMat4("model", model, GL_FALSE);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		
		// TURN ON STENCIL TESTING
		glStencilMask(0xFF); // ENABLE writing to stencil buffer, write bits to stencil buffer as is
		glStencilFunc(GL_ALWAYS, 1, 0xFF); // function signature: glStencilFunc(GLenum func, GLint ref, GLuint mask) | ALL FRAGMENTS SHOULD PASS STENCIL TEST
		glEnable(GL_DEPTH_TEST);

		/*--- Events and buffers ---*/
		glfwSwapBuffers(window.GetWindow());
		glfwPollEvents();
	}

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

	// Shift key makes camera faster.
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

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
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