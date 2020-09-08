#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>
#include "Shader.h"
#include "Texture.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void framebufferSizeCallback(GLFWwindow*, int, int);
void processInput(GLFWwindow*);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
const char* WINDOW_NAME = "big pp";

int main() 
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif // __APPLE__

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, WINDOW_NAME, NULL, NULL);
	if (window == NULL)
	{
		std::cout << "ERROR::INIT::GLFW::WINDOW" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "ERROR::INIT::GLAD" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

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

	//float vertices[] = {
	//	// positions          // colors				// texture coords
	//	 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   2.0f, 2.0f,   // top right
	//	 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   2.0f, 0.0f,   // bottom right
	//	-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
	//	-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 2.0f    // top left 
	//};
	float vertices[] = {
		// positions			// texture coords
		 0.5f,  0.5f, 0.0f,		1.0f, 1.0f,   // top right
		 0.5f, -0.5f, 0.0f,		1.0f, 0.0f,   // bottom right
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,   // bottom left
		-0.5f,  0.5f, 0.0f,		0.0f, 1.0f    // top left 
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

	/*----- SHADERS -----*/
	Shader basicShader("shaders/vertex.vert", "shaders/fragment.frag", SH_PROGRAM);
	basicShader.use();

	//Shader transformShader("shaders/transform.vs", "shaders/transform.fs", SH_PROGRAM);
	
	/*----- TEXTURES -----*/
	Texture woodTexture("textures/wooden-container.jpg", TEXTURE_JPG, true, GL_REPEAT, GL_LINEAR);
	Texture awesomeFaceTexture("textures/awesomeface.png", TEXTURE_PNG, true, GL_MIRRORED_REPEAT, GL_LINEAR);
	Texture crazyCatTexture("textures/crazycat.jpg", TEXTURE_JPG, true, GL_REPEAT, GL_LINEAR);

	/*----- MATH -----*/
	glm::mat4 trans = glm::mat4(1.0f);
	trans = glm::rotate(trans, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
	trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5));
	basicShader.setMat4("transform", trans, GL_FALSE);

	/*----- RENDER (RENDER LOOP) -----*/
	/*
	* Activate shader and set texture uniforms (specify which texture units we are using).
	* We are using GL_TEXTURE0, GL_TEXTURE1 (see render loop).
	*/
	basicShader.setInt("texture1", 0);
	basicShader.setInt("texture2", 1);

	while (!glfwWindowShouldClose(window))
	{
		// input
		processInput(window);

		// rendering commands
		glClearColor(.2f, .3f, .3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		woodTexture.use(GL_TEXTURE0);
		crazyCatTexture.use(GL_TEXTURE1);

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		// check and call events and swap the buffers
		glfwSwapBuffers(window);
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

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}
