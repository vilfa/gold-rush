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
    Skybox(const std::string _directory, 
        const SKYBFORMATenum _format);
	
    void Draw(Shader& shader);

private:
    uint32_t id_;
    uint32_t vao_, vbo_;
    SKYBFORMATenum format_;
    std::string directory_;

    void loadCubemap(const std::vector<std::string> _files);
    void setup();
};

