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
	* Global enables. (glEnable function calls).
	* Use Window::SetGlobalEnable and Window::SetGlobalDisable instead of direct calls.
	*/
	window.SetGlobalEnable(GL_DEPTH_TEST); // Enable depth buffer (depth testing). Don't forget to clear the buffer each frame.
	window.SetGlobalEnable(GL_BLEND); // Enable blending.
	window.SetGlobalEnable(GL_CULL_FACE);

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
		// back face
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // bottom-left
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f, // bottom-right    
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right              
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // bottom-left                
		// front face
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // top-right
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-right        
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // top-right
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f, // top-left        
		// left face
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-right
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-left
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-left       
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-left
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-right
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-right
		// right face
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-left
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right      
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-right          
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-right
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-left
		// bottom face          
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // top-right
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-left
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f, // top-left        
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-left
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // top-right
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-right
		// top face
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // bottom-right                 
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f, // bottom-left  
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f  // top-left              
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

	float grassVertices[] = {
		// positions         // texture Coords (swapped y coordinates because texture is flipped upside down)
		0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
		0.0f, -0.5f,  0.0f,  0.0f,  1.0f,
		1.0f, -0.5f,  0.0f,  1.0f,  1.0f,

		0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
		1.0f, -0.5f,  0.0f,  1.0f,  1.0f,
		1.0f,  0.5f,  0.0f,  1.0f,  0.0f
	};

	// cube VAO
	uint32_t cubeVAO, cubeVBO;
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
	uint32_t planeVAO, planeVBO;
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

	// grass & grass VAO, also used as window positions (with offset)
	std::vector<glm::vec3> vegetation;
	vegetation.push_back(glm::vec3(-1.5f, 0.0f, -0.48f));
	vegetation.push_back(glm::vec3(1.5f, 0.0f, 0.51f));
	vegetation.push_back(glm::vec3(0.0f, 0.0f, 0.7f));
	vegetation.push_back(glm::vec3(-0.3f, 0.0f, -2.3f));
	vegetation.push_back(glm::vec3(0.5f, 0.0f, -0.6f));

	uint32_t grassVAO, grassVBO;
	glGenVertexArrays(1, &grassVAO);
	glGenBuffers(1, &grassVBO);
	glBindVertexArray(grassVAO);
	glBindBuffer(GL_ARRAY_BUFFER, grassVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(grassVertices), &grassVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (const void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (const void*)(3 * sizeof(float)));
	glBindVertexArray(0);

	/*----- SHADERS -----*/
	Shader normalShader("resources/shaders/blending.vert", "resources/shaders/blending.frag");
	
	/*----- TEXTURES -----*/
	uint32_t cubeTexture = loadTexture("resources/textures/container2.png");
	uint32_t floorTexture = loadTexture("resources/textures/wall.jpg");
	uint32_t grassTexture = loadTexture("resources/textures/grass.png");
	uint32_t windowTexture = loadTexture("resources/textures/window.png");

	/*----- MODELS -----*/
	//Model survivalBackpack("resources/models/backpack/backpack.obj");

	/*----- RENDER -----*/
	normalShader.Use();
	normalShader.SetInt("texture1", GL_TEXTURE0);

	// Render parameters
	//glBlendEquation(GL_FUNC_ADD); // This call can be omitted, since GL_FUNC_ADD is the default blend equation.
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glCullFace(GL_BACK); // Tell OpenGL which faces to cull. (default = GL_BACK)
	glFrontFace(GL_CCW); // The front faces are counter-clockwse faces. (default = GL_CCW)

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
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

		/*--- Draw ---*/
		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 projection = glm::perspective(glm::radians(camera.Fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 200.0f);

		normalShader.Use();
		normalShader.SetMat4("view", view, GL_FALSE);
		normalShader.SetMat4("projection", projection, GL_FALSE);

		// DRAW FLOOR
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, floorTexture);
		glBindVertexArray(planeVAO);

		normalShader.SetMat4("model", model, GL_FALSE);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);


		// DRAW CUBES
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, cubeTexture);
		glBindVertexArray(cubeVAO);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
		normalShader.SetMat4("model", model, GL_FALSE);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
		normalShader.SetMat4("model", model, GL_FALSE);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		
		// DRAW GRASS
		view = camera.GetViewMatrix();
		projection = glm::perspective(glm::radians(camera.Fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 200.0f);
		normalShader.SetMat4("view", view, GL_FALSE);
		normalShader.SetMat4("projection", projection, GL_FALSE);
		
		glBindVertexArray(grassVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, grassTexture);
		for (std::size_t i = 0; i < vegetation.size(); i++)
		{
			model = glm::mat4(1.0f);
			model = glm::translate(model, vegetation[i]);
			normalShader.SetMat4("model", model, GL_FALSE);
			glDrawArrays(GL_TRIANGLES, 0, 6);
		}
		
		glBindTexture(GL_TEXTURE_2D, windowTexture);

		// Sort the winows by distance in order to not override the previously drawn windows
		std::map<float, glm::vec3> windowsSortedByDistance; // The map datatype automatically sorts the values by key
		for (std::size_t i = 0; i < vegetation.size(); i++)
		{
			float distance = glm::length(camera.Position - vegetation[i]);
			windowsSortedByDistance.emplace(distance, vegetation[i]); // not usually used this way, but what the hell (also, it's supposedly faster)
			//windowsSortedByDistance.insert({ distance, vegetation[i] });
			//windowsSortedByDistance.insert(std::map<float, glm::vec3>::value_type(distance, vegetation[i]));
		}

		for (std::map<float, glm::vec3>::reverse_iterator it = windowsSortedByDistance.rbegin();
			it != windowsSortedByDistance.rend(); it++)
		{
			model = glm::mat4(1.0f);
			model = glm::translate(model, it->second + glm::vec3(0.0f, 0.0f, 0.1f));
			normalShader.SetMat4("model", model, GL_FALSE);
			glDrawArrays(GL_TRIANGLES, 0, 6);
		}

		/*--- Events and buffers ---*/
		glfwSwapBuffers(window.GetWindow());
		glfwPollEvents();
	}

	/*
	* Clean up.
	*/
	glDeleteVertexArrays(1, &cubeVAO);
	glDeleteVertexArrays(1, &planeVAO);
	glDeleteVertexArrays(1, &grassVAO);
	glDeleteBuffers(1, &cubeVBO);
	glDeleteBuffers(1, &planeVBO);
	glDeleteBuffers(1, &grassVBO);


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