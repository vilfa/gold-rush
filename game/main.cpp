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
#include "Texture.h"
#include "Camera.h"

void framebufferSizeCallback(GLFWwindow*, int, int);
void processInput(GLFWwindow*, Camera*, float);
void mouseMoveCallback(GLFWwindow*, double, double);
void mouseScrollCallback(GLFWwindow*, double, double);

/*
* Window setup
*/
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
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
* Light source setup
*/
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

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
	glEnable(GL_DEPTH_TEST);

	/*----- GEOMETRY -----*/
	float vertices[] = {
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
	};

	unsigned int objectVAO, VBO;
	glGenVertexArrays(1, &objectVAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(objectVAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	unsigned int lightSourceVAO;
	glGenVertexArrays(1, &lightSourceVAO);
	glBindVertexArray(lightSourceVAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	/*----- SHADERS -----*/
	Shader lightSourceShader("shaders/vertex_light_source.vert", "shaders/fragment_light_source.frag");
	Shader objectShader("shaders/vertex_light.vert", "shaders/fragment_light.frag");
	
	/*----- TEXTURES -----*/
	//Texture woodTexture("textures/wooden-container.jpg", TXenum::TEXTURE_JPG, true, GL_REPEAT, GL_LINEAR);
	//Texture awesomeFaceTexture("textures/awesomeface.png", TXenum::TEXTURE_PNG, true, GL_MIRRORED_REPEAT, GL_LINEAR);
	//Texture crazyCatTexture("textures/crazycat.jpg", TXenum::TEXTURE_JPG, true, GL_REPEAT, GL_LINEAR);
	/*
	* Activate shader and set texture uniforms (specify which texture units we are using).
	* We are using GL_TEXTURE0, GL_TEXTURE1 (see render loop).
	*/
	//basicShader.SetInt("texture1", 0);
	//basicShader.SetInt("texture2", 1);
	
	/*----- RENDER -----*/
	while (!window.GetWindowShouldClose())
	{
		/*--- Frame time logic ---*/
		double currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		/*--- Input ---*/
		processInput(window.GetWindow(), &camera, (float)deltaTime);

		/*--- Render commands ---*/
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear color and depth buffer
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

		/*--- View and projection transformations ---*/
		glm::mat4 projection = glm::perspective(glm::radians(camera.Fov),
			(float)window.GetWidth() / (float)window.GetHeight(),
			0.1f, 200.0f);

		/*--- Draw ---*/
		// CUBE
		objectShader.Use();

		// Light properties
		lightPos = glm::vec3(2.5f * cos(glfwGetTime()), 0.0f, 2.5f * sin(glfwGetTime()));
		objectShader.SetVec3("light.position", lightPos);
		objectShader.SetVec3("light.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
		objectShader.SetVec3("light.diffuse", glm::vec3(0.5f, 0.5f, 0.5f));
		objectShader.SetVec3("light.specular", glm::vec3(1.0f, 1.0f, 1.0f));
		
		// Material properties
		objectShader.SetVec3("material.ambient", glm::vec3(0.1745, 0.01175, 0.01175));
		objectShader.SetVec3("material.diffuse", glm::vec3(0.61424, 0.04136, 0.04136));
		objectShader.SetVec3("material.specular", glm::vec3(0.727811, 0.626959, 0.626959));
		objectShader.SetFloat("material.shininess", 0.6f);
		
		// Camera properties
		objectShader.SetVec3("viewPos", camera.Position);
		objectShader.SetMat4("projection", projection, false);
		objectShader.SetMat4("view", camera.GetViewMatrix(), false);

		// World transformations
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::rotate(model, 100.0f * glm::radians((float)glfwGetTime()), glm::vec3(1.0f, 1.0f, 1.0f));
		objectShader.SetMat4("model", model, false);

		// Render cube
		glBindVertexArray(objectVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		// !CUBE

		// LIGHT CUBE
		lightSourceShader.Use();

		glm::vec3 lightSourceColor = glm::vec3();
		lightSourceColor.x = randomDistribution(randomEngine);
		lightSourceColor.y = randomDistribution(randomEngine);
		lightSourceColor.z = randomDistribution(randomEngine);
		lightSourceShader.SetVec3("lightSourceColor", lightSourceColor);
		lightSourceShader.SetMat4("projection", projection, false);
		lightSourceShader.SetMat4("view", camera.GetViewMatrix(), false);
		model = glm::mat4(1.0f);
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.25f));
		lightSourceShader.SetMat4("model", model, false);
		
		glBindVertexArray(lightSourceVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		// !LIGHT CUBE

		/*--- Events and buffers ---*/
		glfwSwapBuffers(window.GetWindow());
		glfwPollEvents();
	}

	/*
	* Unallocate buffers and terminate program.
	*/
	glDeleteVertexArrays(1, &objectVAO);
	glDeleteVertexArrays(1, &lightSourceVAO);
	glDeleteBuffers(1, &VBO);
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
		speed = CAMSPDenum::CAMERA_FAST;
	}
	else
	{
		speed = CAMSPDenum::CAMERA_NORMAL;
	}

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		camera->ProcessKeyboard(CAMMOVenum::CAMERA_FORWARD, speed, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		camera->ProcessKeyboard(CAMMOVenum::CAMERA_BACKWARD, speed, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		camera->ProcessKeyboard(CAMMOVenum::CAMERA_LEFT, speed, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		camera->ProcessKeyboard(CAMMOVenum::CAMERA_RIGHT, speed, deltaTime);
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
	camera.ProcessMouseScroll((float)yOffset);
}