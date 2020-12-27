#pragma once

#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Types/AABB.h"
#include "World/TerrainElement.h"

class Entity
{
public:
    AABB bounding_box_;

    Entity(TerrainElement& terr_el, glm::mat4& world_transform);

    void Draw(glm::vec3 position, float yaw);

    AABB GetBoundingBox();
    bool Collides(Entity oth_ent);
    bool Contains(glm::vec3 oth_pos);

    glm::vec3 GetCenter();
    float GetXMaxAABB();
    float GetXMinAABB();
    float GetYMaxAABB();
    float GetYMinAABB();
    float GetZMaxAABB();
    float GetZMinAABB();

private:
    TerrainElement terrain_element_;
    glm::mat4 world_transform_;

    void setupBoundingBox();
};
