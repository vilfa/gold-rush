#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

struct AABB
{
	glm::vec3 center_position;
	float x_half_dim;
	float y_half_dim;
	float z_half_dim;
};