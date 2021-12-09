#pragma once

#include <iostream>
#include <limits>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Renderer/Model.h"
#include "Types/AABB.h"

class GObject
{
public:
    AABB model_bounding_box_;

    GObject(Model obj_model);

    void Draw(Shader &shader);
    void Draw(Shader &shader, glm::vec3 position);
    void Draw(Shader &shader, glm::vec3 position, float yaw);
    void DrawInstanced(Shader &shader, std::vector<glm::mat4> &instance_mod_mats);
    void DrawInstanced(Shader &shader, std::shared_ptr<std::vector<glm::mat4>> instance_mod_mats);

    AABB GetModelBoundingBox();

    float GetXMaxModelAABB();
    float GetXMinModelAABB();
    float GetYMaxModelAABB();
    float GetYMinModelAABB();
    float GetZMaxModelAABB();
    float GetZMinModelAABB();

protected:
    Model model_;
    glm::vec3 world_position_;

    void calculateModelBoundingBox();
};
