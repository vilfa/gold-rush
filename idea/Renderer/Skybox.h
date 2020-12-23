#pragma once

#include <iostream>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>

#include "Renderer/Shader.h"
#include "Types/ESkybox.h"

class Skybox
{
public:
    Skybox(
        const std::string directory, 
        const SKYBFORMATenum format
    );
	
    void Draw(Shader& shader);

private:
    uint32_t ID;
    uint32_t VAO, VBO;
    SKYBFORMATenum format;
    std::string directory;

    void loadCubemap(const std::vector<std::string> files);
    void setup();
};

