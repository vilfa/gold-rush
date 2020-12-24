#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include <iostream>
#include <random>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Application/Window.h"
#include "Renderer/Shader.h"
#include "Renderer/Camera.h"
#include "Renderer/Model.h"
#include "Renderer/Renderer.h"
#include "Terrain/NoiseGenerator.h"

void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void mouseMoveCallback(GLFWwindow* window, double xPos, double yPos);
void mouseScrollCallback(GLFWwindow* window, double xOffset, double yOffset);

/*
* Window setup
*/
const uint32_t SCR_WIDTH = 1600;
const uint32_t SCR_HEIGHT = 900;
const std::string WINDOW_NAME = "Idea";

Window window(SCR_WIDTH, SCR_HEIGHT, WINDOW_NAME, 4, 2, true);
Renderer renderer(window);

int main()
{
	window.SetFramebufferSizeCallback(framebufferSizeCallback);
	window.SetMouseMoveCallback(mouseMoveCallback);
	window.SetMouseScrollCallback(mouseScrollCallback);

	renderer.Render();
	_CrtDumpMemoryLeaks();
	return 0;
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	renderer.FramebufferSizeCallback(window, width, height);
}

void mouseMoveCallback(GLFWwindow* window, double xPos, double yPos)
{
	renderer.MouseMoveCallback(window, xPos, yPos);
}

void mouseScrollCallback(GLFWwindow* window, double xOffset, double yOffset)
{
	renderer.MouseScrollCallback(window, xOffset, yOffset);
}