#pragma once

#include <iostream>
#include <vector>
#include <limits>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Renderer/Model.h"

struct AABB
{
	glm::vec3 center_position;
	float x_half_dim;
	float y_half_dim;
	float z_half_dim;
};

class GObject
{
public:
	AABB bounding_box_;

	GObject(Model obj_model);

	void Draw(Shader& shader);
	void DrawInstanced(Shader& shader, std::vector<glm::mat4>& instance_mod_mats);
	void DrawInstanced(Shader& shader, std::shared_ptr<std::vector<glm::mat4>> instance_mod_mats);

	AABB GetBoundingBox();

	float GetXMaxAABB();
	float GetXMinAABB();
	float GetYMaxAABB();
	float GetYMinAABB();
	float GetZMaxAABB();
	float GetZMinAABB();

	glm::vec3 GetWorldPosition();
	void SetWorldPosition(glm::vec3 new_world_pos);

	bool CollidesAABB(GObject oth_obj);
	bool ContainsAABB(glm::vec3 oth_pos);

protected:
	Model model_;
	glm::vec3 world_position_;

	void calculateBoundingBox();
};
