#include <iostream>

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
void processInput(GLFWwindow*, Camera&, double);

/*
* Window setup
*/
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
const std::string WINDOW_NAME = "big pp";

/*
* Mouse setup
*/
bool firstMouse = true;
float lastX = SCR_WIDTH / 2;
float lastY = SCR_HEIGHT / 2;

/*
* Camera setup
*/
double deltaTime = 0.0;
double lastFrame = 0.0;

int main() 
{
	Window window(SCR_WIDTH, SCR_HEIGHT, WINDOW_NAME);
	glfwSetFramebufferSizeCallback(window.getWindow(), framebufferSizeCallback);
	window.setInputMode(GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window.getWindow(), mouseMoveCallback);
	glfwSetScrollCallback(window.getWindow(), mouseScrollCallback);

	Camera camera(&window);

	/*----- GEOMETRY -----*/
	/*
	* Create VAO (Vertex Array Object) to store different buffers.
	* You can bind all different kinds of buffers to the VAO!
	*/
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	unsigned int VBO, EBO;
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	float vertices[] = {
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
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	unsigned int indices[] = {
		0, 1, 3,
		1, 2, 3,
	};
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	/*
	* Tell OpenGL how to interperet the VAO (in this case: 4B/32b floats).
	*/
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glm::vec3 cubePositions[] = {
		glm::vec3(0.1f,  0.1f,  0.5f),
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

	/*----- TEXTURES -----*/
	Texture woodTexture("textures/wooden-container.jpg", TEXTURE_JPG, true, GL_REPEAT, GL_LINEAR);
	Texture awesomeFaceTexture("textures/awesomeface.png", TEXTURE_PNG, true, GL_MIRRORED_REPEAT, GL_LINEAR);
	Texture crazyCatTexture("textures/crazycat.jpg", TEXTURE_JPG, true, GL_REPEAT, GL_LINEAR);

	/*----- SHADERS -----*/
	Shader basicShader("shaders/vertex.vert", "shaders/fragment.frag", SH_PROGRAM);
	basicShader.use();
	/*
	* Activate shader and set texture uniforms (specify which texture units we are using).
	* We are using GL_TEXTURE0, GL_TEXTURE1 (see render loop).
	*/
	basicShader.setInt("texture1", 0);
	basicShader.setInt("texture2", 1);

	/*----- RENDER -----*/
	/*
	* Enable depth buffer (depth testing). The buffer must be cleared for each frame
	* so we don't store the information of the previous frame.
	*/
	glEnable(GL_DEPTH_TEST);

	/*----- RENDER LOOP -----*/
	while (!window.getWindowShouldClose())
	{
		double currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		std::cout << "FPS:" << 1 / deltaTime << "|" << "Frame Time:" << deltaTime * 1000 << std::endl;

		// Input
		processInput(window.getWindow(), camera, currentFrame);

		// Rendering commands
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear colour and depth buffer
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glClearColor(.2f, .3f, .3f, 1.0f);
		
		woodTexture.use(GL_TEXTURE0);
		crazyCatTexture.use(GL_TEXTURE1);

		basicShader.setMat4("view", camera.getViewMatrix(), false);
		basicShader.setMat4("projection", camera.getProjectionMatrix(), false);

		// Draw
		glBindVertexArray(VAO);
		for (unsigned int i = 0; i < 10; i++)
		{
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			float angle = 20.0f * i + 50.0f;
			if (i % 3 == 0)
			{
				model = glm::rotate(model, (float)glfwGetTime() * glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			}
			basicShader.setMat4("model", model, false);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		// Check and call events and swap the buffers
		glfwSwapBuffers(window.getWindow());
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glfwTerminate();
	return 0;
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window, Camera& camera, double deltaTime)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		camera.processKeyboard(CAMERA_FORWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		camera.processKeyboard(CAMERA_BACKWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		camera.processKeyboard(CAMERA_LEFT, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		camera.processKeyboard(CAMERA_RIGHT, deltaTime);
	}
}

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

	
}

void mouseScrollCallback(GLFWwindow* window, double xOffset, double yOffset)
{
	
}