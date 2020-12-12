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

int main()
{
	/*----- WINDOW & GLOBAL ATTRIBUTES -----*/
	Window window(SCR_WIDTH, SCR_HEIGHT, WINDOW_NAME, 4, 2, true, 4); // Use OpenGL 4.2, and enable 4xMSAA.
	window.SetFramebufferSizeCallback(framebufferSizeCallback);
	window.SetInputMode(GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	window.SetMouseMoveCallback(mouseMoveCallback);
	window.SetMouseScrollCallback(mouseScrollCallback);

	/*
	* Global enables. (glEnable function calls).
	* Use Window::SetGlobalEnable and Window::SetGlobalDisable instead of direct calls.
	* Makes the code more readable (will probably move this to a e.g. Renderer class).
	* Update: not using the Window methods anymore, will use standard glEnable until I implement
	* the Renderer class.
	*/
	glEnable(GL_DEPTH_TEST);
	
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK); // Tell OpenGL which faces to cull. (default = GL_BACK)
	glFrontFace(GL_CCW); // The front faces are counter-clockwse faces. (default = GL_CCW)
	
	glEnable(GL_BLEND);
	glBlendEquation(GL_FUNC_ADD); // This call can be omitted, since GL_FUNC_ADD is the default blend equation.
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_MULTISAMPLE); // Enable anti-aliasing. The technique used is MSAA (Multi Sample Anti-Aliasing).

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

	//uint32_t nAsteroids = 50000;
	//glm::mat4* instanceModelMatrices = new glm::mat4[nAsteroids];
	//srand(glfwGetTime());
	//float radius = 75.0f;
	//float offset = 25.0f;
	//for (std::size_t i = 0; i < nAsteroids; i++)
	//{
	//	glm::mat4 model = glm::mat4(1.0f);
	//	// 1. translation: displace along circle with 'radius' in range [-offset, offset]
	//	float angle = (float)i / (float)nAsteroids * 360.0f;
	//	float displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
	//	float x = sin(angle) * radius + displacement;
	//	displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
	//	float y = displacement * 0.4f; // keep height of field smaller compared to width of x and z
	//	displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
	//	float z = cos(angle) * radius + displacement;
	//	model = glm::translate(model, glm::vec3(x, y, z));

	//	// 2. scale: scale between 0.05 and 0.25f
	//	float scale = (rand() % 20) / 100.0f + 0.05f;
	//	model = glm::scale(model, glm::vec3(scale));

	//	// 3. rotation: add random rotation around a (semi) randomly picked rotation axis vector
	//	float rotAngle = (rand() % 360);
	//	model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));
	//	
	//	instanceModelMatrices[i] = model;
	//}

	/*----- SHADERS -----*/
	Shader skyboxShader("resources/shaders/skybox_shader.vert", "resources/shaders/skybox_shader.frag");
	Shader backpackShader("resources/shaders/backpack_shader.vert", "resources/shaders/backpack_shader.frag");
	Shader explodingBackpackShader("resources/shaders/backpack_shader_ubo.vert", "resources/shaders/explode_object.geom", "resources/shaders/backpack_shader_ubo.frag");
	Shader normalsBackpackShader("resources/shaders/backpack_with_normals.vert", "resources/shaders/backpack_with_normals.geom", "resources/shaders/backpack_with_normals.frag");
	Shader instancedQuadsShader("resources/shaders/instanced_quads.vert", "resources/shaders/instanced_quads.frag");
	Shader marsShader("resources/shaders/mars.vert", "resources/shaders/mars.frag"); 
	Shader asteroidShader("resources/shaders/asteroid.vert", "resources/shaders/asteroid.frag");

	/*----- UNIFORM BLOCKS ------*/
	uint32_t uboMatrices;
	glGenBuffers(1, &uboMatrices);
	glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices); // The target is GL_UNIFORM_BUFFER.
	glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW); // We'll use this for the view and projection matrix so 2 * sizeof(glm::mat4)
	glBindBuffer(GL_UNIFORM_BUFFER, 0); // Unbind.

	// Binding point 0 is already EXPLICITLY SET IN THE SHADER (version 420 core and up).
	glBindBufferRange(GL_UNIFORM_BUFFER, 0, uboMatrices, 0, 2 * sizeof(glm::mat4)); // Bind the entire buffer to binding point 0.
	std::cout << "INFO::MAIN::GL_UNIFORM_BUFFER::GL_MAX_VERTEX_UNIFORM_COMPONENTS" << std::endl;
	std::cout << "Max uniform components:" << (int) GL_MAX_VERTEX_UNIFORM_COMPONENTS << std::endl;
	
	/*----- TEXTURES -----*/
	std::vector<std::string> skyboxSeaPaths
	{
		"resources/skybox/sea/right.jpg",
		"resources/skybox/sea/left.jpg",
		"resources/skybox/sea/top.jpg",
		"resources/skybox/sea/bottom.jpg",
		"resources/skybox/sea/front.jpg",
		"resources/skybox/sea/back.jpg"
	};
	std::vector<std::string> skyboxCityPaths
	{
		"resources/skybox/Yokohama3/posx.jpg",
		"resources/skybox/Yokohama3/negx.jpg",
		"resources/skybox/Yokohama3/posy.jpg",
		"resources/skybox/Yokohama3/negy.jpg",
		"resources/skybox/Yokohama3/posz.jpg",
		"resources/skybox/Yokohama3/negz.jpg"
	};
	std::vector<std::string> skyboxSpacePaths
	{
		"resources/skybox/space/right.png",
		"resources/skybox/space/left.png",
		"resources/skybox/space/top.png",
		"resources/skybox/space/bottom.png",
		"resources/skybox/space/front.png",
		"resources/skybox/space/back.png"
	};

	uint32_t skyboxSeaTexture = loadCubemap(skyboxSeaPaths);
	uint32_t skyboxCityTexture = loadCubemap(skyboxCityPaths);
	uint32_t skyboxSpaceTexture = loadCubemap(skyboxSpacePaths);

	/*----- MODELS -----*/
	//Model survivalBackpack("resources/models/backpack/backpack.obj");
	Model mars("resources/models/mars/mars.obj");
	//Model asteroid("resources/models/rock/rock.obj");

	/*uint32_t instanceModelMatVBO;
	glGenBuffers(1, &instanceModelMatVBO);
	glBindBuffer(GL_ARRAY_BUFFER, instanceModelMatVBO);
	glBufferData(GL_ARRAY_BUFFER, nAsteroids * sizeof(glm::mat4), instanceModelMatrices, GL_STATIC_DRAW);*/

	/*for (std::size_t i = 0; i < asteroid.meshes.size(); i++)
	{
		uint32_t VAO = asteroid.meshes.at(i).VAO;
		glBindVertexArray(VAO);

		std::size_t vec4Size = sizeof(glm::vec4);

		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (const void*)0);
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (const void*)(1 * vec4Size));
		glEnableVertexAttribArray(5);
		glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (const void*)(2 * vec4Size));
		glEnableVertexAttribArray(6);
		glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (const void*)(3 * vec4Size));

		glVertexAttribDivisor(3, 1);
		glVertexAttribDivisor(4, 1);
		glVertexAttribDivisor(5, 1);
		glVertexAttribDivisor(6, 1);

		glBindVertexArray(0);
	}*/

	/*----- RENDER -----*/
	skyboxShader.Use();
	skyboxShader.SetInt("skybox", 0); // Init skybox.

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
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Not using the stencil buffer right now.
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

		/*--- Draw ---*/
		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 projection = glm::perspective(glm::radians(camera.Fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 200.0f);

		// Bind view and projection matrix to the uniform buffer.
		glBindBuffer(GL_UNIFORM_BUFFER,	uboMatrices);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(projection));
		glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(view));
		glBindBuffer(GL_UNIFORM_BUFFER, 0);

		// Mars
		marsShader.Use();
		marsShader.SetMat4("model", model);
		mars.Draw(marsShader);

		// Asteroids
		/*asteroidShader.Use();
		asteroidShader.SetInt("texture_diffuse_1", 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, asteroid.texturesLoaded.at(0).id);
		for (std::size_t i = 0; i < asteroid.meshes.size(); i++)
		{
			glBindVertexArray(asteroid.meshes.at(i).VAO);
			glDrawElementsInstanced(
				GL_TRIANGLES, asteroid.meshes.at(i).Indices.size(), GL_UNSIGNED_INT, (const void*)0, nAsteroids
			);
			glBindVertexArray(0);
		}*/

		// Skybox
		glDepthFunc(GL_LEQUAL);
		glBindVertexArray(skyboxVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxSpaceTexture);

		skyboxShader.Use();
		view = glm::mat4(glm::mat3(camera.GetViewMatrix()));
		skyboxShader.SetMat4("view", view);
		skyboxShader.SetMat4("projection", projection);
		
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glDepthFunc(GL_LESS);

		/*--- Events and buffers ---*/
		glfwSwapBuffers(window.GetWindow());
		glfwPollEvents();
	}

	/*
	* Clean up.
	*/
	glDeleteVertexArrays(1, &skyboxVAO);
	glDeleteBuffers(1, &skyboxVBO);
	glDeleteBuffers(1, &uboMatrices);
	//glDeleteBuffers(1, &instanceModelMatVBO);

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
		lastX = (float) xPos;
		lastY = (float) yPos;
		firstMouse = false;
	}

	float xOffset = (float) xPos - lastX;
	float yOffset = lastY - (float) yPos; // Reversed since y-coordinates range from bottom to top.
	lastX = (float) xPos;
	lastY = (float) yPos;

	camera.ProcessMouseMovement(xOffset, yOffset);
}

/* 
* Process mouse scroll input.
*/
void mouseScrollCallback(GLFWwindow* window, double xOffset, double yOffset)
{
	camera.ProcessMouseScroll((float&) yOffset);
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
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nComp;
	unsigned char* data;
	for (std::size_t i = 0; i < paths.size(); i++)
	{
		data = stbi_load(paths[i].c_str(), &width, &height, &nComp, 0);
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

			glTexImage2D(
				GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data
			);

			stbi_image_free(data);
		}
		else
		{
			std::cout << "ERROR::MAIN::LOAD_CUBEMAP::CUBEMAP_LOAD_ERROR" << std::endl;
			std::cout << "Cubemap load failed at path:" << paths[i] << std::endl;
			stbi_image_free(data);
		}
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	return textureID;
}