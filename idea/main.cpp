//#define _CRTDBG_MAP_ALLOC
//#include <stdlib.h>
//#include <crtdbg.h>

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

const uint32_t _SCR_WIDTH = 1600;
const uint32_t _SCR_HEIGHT = 900;
const std::string _WINDOW_NAME = "Idea";

Window WINDOW(_SCR_WIDTH, _SCR_HEIGHT, _WINDOW_NAME, 4, 2, true);
Renderer RENDERER(WINDOW);

int main()
{
	WINDOW.SetFramebufferSizeCallback(framebufferSizeCallback);
	WINDOW.SetMouseMoveCallback(mouseMoveCallback);
	WINDOW.SetMouseScrollCallback(mouseScrollCallback);

	RENDERER.Render();
	//_CrtDumpMemoryLeaks();
	return 0;
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	RENDERER.FramebufferSizeCallback(window, width, height);
}

void mouseMoveCallback(GLFWwindow* window, double x_pos, double y_pos)
{
	RENDERER.MouseMoveCallback(window, x_pos, y_pos);
}

void mouseScrollCallback(GLFWwindow* window, double x_offset, double y_offset)
{
	RENDERER.MouseScrollCallback(window, x_offset, y_offset);
}