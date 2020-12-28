#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Application/Window.h"
#include "Game/Game.h"

void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void mouseMoveCallback(GLFWwindow* window, double xPos, double yPos);

const uint32_t _SCR_WIDTH = 1600;
const uint32_t _SCR_HEIGHT = 900;
const std::string _WINDOW_NAME = "Squirrel Gold Rush";

Window WINDOW(_SCR_WIDTH, _SCR_HEIGHT, _WINDOW_NAME, 4, 2, true);
Game GAME(WINDOW);

int main()
{
	WINDOW.SetFramebufferSizeCallback(framebufferSizeCallback);
	WINDOW.SetMouseMoveCallback(mouseMoveCallback);

	GAME.Start();
	return 0;
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	GAME.HandleFramebuffer(window, width, height);
}

void mouseMoveCallback(GLFWwindow* window, double x_pos, double y_pos)
{
	GAME.HandleMouse(window, x_pos, y_pos);
}