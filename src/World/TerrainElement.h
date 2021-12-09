#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Renderer/Model.h"
#include "Renderer/Shader.h"
#include "World/GObject.h"

class TerrainElement : public virtual GObject
{
public:
    TerrainElement(Model model, Shader shader);

    void Draw();
    void Draw(glm::vec3 position);
    void Draw(glm::vec3 position, float yaw);
    void DrawInstanced(std::vector<glm::mat4> &instance_mod_mats);
    void DrawInstanced(std::shared_ptr<std::vector<glm::mat4>> instance_mod_mats);

private:
    Shader shader_;
};
