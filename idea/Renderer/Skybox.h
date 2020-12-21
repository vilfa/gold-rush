#pragma once

#include <iostream>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>

#include "Renderer/Shader.h"

enum class SKYBFORMATenum
{
    JPG,
    PNG
};

class Skybox
{
public:
    Skybox(const std::string directory, const SKYBFORMATenum format, const bool isMultipleFiles = true);
	void Draw(Shader& shader);

private:
    uint32_t ID;
    uint32_t VAO, VBO;
    std::string directory;
    SKYBFORMATenum format;

    void loadCubemap(const std::vector<std::string> files);
    void prepare();
};

