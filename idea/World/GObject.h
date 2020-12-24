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
	glm::vec3 CenterPosition;
	float XHalfDim;
	float YHalfDim;
	float ZHalfDim;
};

class GObject
{
public:
	AABB BoundingBox;

	GObject(Model objectModel);

	void Draw(Shader& shader);
	void DrawInstanced(Shader& shader, std::vector<glm::mat4>& instanceMMats);
	void DrawInstanced(Shader& shader, std::shared_ptr<std::vector<glm::mat4>> instanceMMats);

	AABB GetBoundingBox();

	float GetXMaxAABB();
	float GetXMinAABB();
	float GetYMaxAABB();
	float GetYMinAABB();
	float GetZMaxAABB();
	float GetZMinAABB();

	glm::vec3 GetWorldPosition();
	void SetWorldPosition(glm::vec3 newPosition);

	bool CollidesAABB(GObject oObj);
	bool ContainsAABB(glm::vec3 oPos);

protected:
	Model GModel;
	glm::vec3 WPosition;

	void CalculateBoundingBox();
};
