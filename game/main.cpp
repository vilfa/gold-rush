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

	/*----- GEOMETRY -----*/
	float vertices[] = {
		// positions          // normals           // texture coords
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
	};

	unsigned int objectVAO, VBO;
	glGenVertexArrays(1, &objectVAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(objectVAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	unsigned int lightSourceVAO;
	glGenVertexArrays(1, &lightSourceVAO);
	glBindVertexArray(lightSourceVAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	/*----- SHADERS -----*/
	Shader lightSourceShader("resources/shaders/vertex_light_source.vert", "resources/shaders/fragment_light_source.frag");
	Shader objectShader("resources/shaders/vertex_light.vert", "resources/shaders/fragment_light.frag");
	
	/*----- TEXTURES -----*/
	//Texture woodTexture("textures/wooden-container.jpg", TXenum::TEXTURE_JPG, true, GL_REPEAT, GL_LINEAR);
	//Texture awesomeFaceTexture("textures/awesomeface.png", TXenum::TEXTURE_PNG, true, GL_MIRRORED_REPEAT, GL_LINEAR);
	//Texture crazyCatTexture("textures/crazycat.jpg", TXenum::TEXTURE_JPG, true, GL_REPEAT, GL_LINEAR);
	//Texture diffuseMap("resources/textures/container2.png", TXenum::TEXTURE_PNG, true, GL_REPEAT, GL_LINEAR);
	//Texture specularMap("resources/textures/container2_specular.png", TXenum::TEXTURE_PNG, true, GL_REPEAT, GL_LINEAR);
	/*
	* Activate shader and set texture uniforms (specify which texture units we are using).
	* We are using GL_TEXTURE0, GL_TEXTURE1 (see render loop).
	*/
	//basicShader.SetInt("texture1", 0);
	//basicShader.SetInt("texture2", 1);
	
	glm::vec3 cubePositions[] = {
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
	};

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
		// 1. Directional light
		objectShader.SetVec3("directionalLight.direction", glm::vec3(-0.2f, -1.0f, -0.3f));
		objectShader.SetVec3("directionalLight.ambient", glm::vec3(0.05f, 0.05f, 0.05f));
		objectShader.SetVec3("directionalLight.diffuse", glm::vec3(0.4f, 0.4f, 0.4f));
		objectShader.SetVec3("directionalLight.specular", glm::vec3(0.5f, 0.5f, 0.5f));
		// 2. Point lights
		// point light 1
		objectShader.SetVec3("pointLights[0].position", pointLightPositions[0]);
		objectShader.SetVec3("pointLights[0].ambient", glm::vec3(0.05f, 0.05f, 0.05f));
		objectShader.SetVec3("pointLights[0].diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
		objectShader.SetVec3("pointLights[0].specular", glm::vec3(1.0f, 1.0f, 1.0f));
		objectShader.SetFloat("pointLights[0].constant", 1.0f);
		objectShader.SetFloat("pointLights[0].linear", 0.09f);
		objectShader.SetFloat("pointLights[0].quadratic", 0.032f);
		// point light 2
		objectShader.SetVec3("pointLights[1].position", pointLightPositions[1]);
		objectShader.SetVec3("pointLights[1].ambient", glm::vec3(0.05f, 0.05f, 0.05f));
		objectShader.SetVec3("pointLights[1].diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
		objectShader.SetVec3("pointLights[1].specular", glm::vec3(1.0f, 1.0f, 1.0f));
		objectShader.SetFloat("pointLights[1].constant", 1.0f);
		objectShader.SetFloat("pointLights[1].linear", 0.09f);
		objectShader.SetFloat("pointLights[1].quadratic", 0.032f);
		// point light 3
		objectShader.SetVec3("pointLights[2].position", pointLightPositions[2]);
		objectShader.SetVec3("pointLights[2].ambient", glm::vec3(0.05f, 0.05f, 0.05f));
		objectShader.SetVec3("pointLights[2].diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
		objectShader.SetVec3("pointLights[2].specular", glm::vec3(1.0f, 1.0f, 1.0f));
		objectShader.SetFloat("pointLights[2].constant", 1.0f);
		objectShader.SetFloat("pointLights[2].linear", 0.09f);
		objectShader.SetFloat("pointLights[2].quadratic", 0.032f);
		// point light 4
		objectShader.SetVec3("pointLights[3].position", pointLightPositions[3]);
		objectShader.SetVec3("pointLights[3].ambient", glm::vec3(0.05f, 0.05f, 0.05f));
		objectShader.SetVec3("pointLights[3].diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
		objectShader.SetVec3("pointLights[3].specular", glm::vec3(1.0f, 1.0f, 1.0f));
		objectShader.SetFloat("pointLights[3].constant", 1.0f);
		objectShader.SetFloat("pointLights[3].linear", 0.09f);
		objectShader.SetFloat("pointLights[3].quadratic", 0.032f);
		// 3. Spot light
		objectShader.SetVec3("spotLight.position", camera.Position);
		objectShader.SetVec3("spotLight.direction", camera.Front);
		objectShader.SetVec3("spotLight.ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		objectShader.SetVec3("spotLight.diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
		objectShader.SetVec3("spotLight.specular", glm::vec3(1.0f, 1.0f, 1.0f));
		objectShader.SetFloat("spotLight.constant", 1.0f);
		objectShader.SetFloat("spotLight.linear", 0.09f);
		objectShader.SetFloat("spotLight.quadratic", 0.032f);
		objectShader.SetFloat("spotLight.cutoff", glm::cos(glm::radians(12.5f)));
		objectShader.SetFloat("spotLight.outerCutoff", glm::cos(glm::radians(15.0f)));

		// Material properties
		objectShader.SetInt("material.diffuse", 0);
		objectShader.SetInt("material.specular", 1);
		objectShader.SetFloat("material.shininess", 64.0f);
		
		// Camera properties
		objectShader.SetVec3("viewPosition", camera.Position);
		objectShader.SetMat4("projection", projection, false);
		objectShader.SetMat4("view", camera.GetViewMatrix(), false);

		// World transformations
		glm::mat4 model = glm::mat4(1.0f);
		objectShader.SetMat4("model", model, false);

		// Bind diffuse map
		//diffuseMap.Use(GL_TEXTURE0);
		//specularMap.Use(GL_TEXTURE1);
		
		// Render cube
		glBindVertexArray(objectVAO);

		for (unsigned int i = 0; i < 10; i++)
		{
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			float angle = 20.0f * i;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			objectShader.SetMat4("model", model, false);

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		//glDrawArrays(GL_TRIANGLES, 0, 36);
		// !CUBE

		
		// LIGHT CUBES
		lightSourceShader.Use();

		glm::vec3 lightSourceColor = glm::vec3(1.0f);
		/*lightSourceColor.x = randomDistribution(randomEngine);
		lightSourceColor.y = randomDistribution(randomEngine);
		lightSourceColor.z = randomDistribution(randomEngine);*/
		lightSourceShader.SetVec3("lightSourceColor", lightSourceColor);
		lightSourceShader.SetMat4("projection", projection, false);
		lightSourceShader.SetMat4("view", camera.GetViewMatrix(), false);
		
		glBindVertexArray(lightSourceVAO);
		for (unsigned int i = 0; i < 4; i++)
		{
			model = glm::mat4(1.0f);
			model = glm::translate(model, pointLightPositions[i]);
			model = glm::scale(model, glm::vec3(0.25f));
			lightSourceShader.SetMat4("model", model, false);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
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