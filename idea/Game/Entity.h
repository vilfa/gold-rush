#pragma once

#include <iostream>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Types/SBoundingBox.h"
#include "World/TerrainElement.h"

class Entity
{
public:
    Entity(TerrainElement& terr_el, glm::mat4& world_transform);

    AABB& GetBoundingBox();
    bool Collides(Entity oth_ent);
    bool Contains(glm::vec3 oth_pos);

    float GetXMaxAABB();
    float GetXMinAABB();
    float GetYMaxAABB();
    float GetYMinAABB();
    float GetZMaxAABB();
    float GetZMinAABB();

private:
    AABB bounding_box_;

    TerrainElement& terrain_element_;
    glm::mat4& world_transform_;

    void setupBoundingBox();
};

