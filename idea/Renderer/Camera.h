#pragma once

#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Application/Window.h"
#include "Types/ECamera.h"

class Camera
{
public:
	const float _aspect_ratio_;
	glm::vec3 world_up_;
	glm::vec3 position_;
	glm::vec3 front_;
	glm::vec3 up_;
	glm::vec3 right_;
	float frustum_near_;
	float frustum_far_;
	float yaw_;
	float pitch_;
	float movement_speed_;
	float movement_speed_fast_;
	float mouse_sensitivity_;
	float fov_;

	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), 
		float aspect_ratio = _ASPECT_RATIO_,
		glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
		float frustum_near = _FRUSTUM_NEAR_, 
		float frustum_far = _FRUSTUM_FAR_, 
		float yaw = _YAW_, 
		float pitch = _PITCH_);

	Camera(float pos_x, 
		float pos_y, 
		float pos_z, 
		float up_x, 
		float up_y, 
		float up_z, 
		float yaw, 
		float pitch, 
		float aspect_ratio = _ASPECT_RATIO_,
		float frustum_near = _FRUSTUM_NEAR_, 
		float frustum_far = _FRUSTUM_FAR_);

	glm::mat4 GetViewMatrix() const;
	glm::mat3 GetViewMatrix3() const;
	glm::mat4 GetProjectionMatrix() const;
	glm::mat4 GetProjectionViewMatrix() const;
	void ProcessKeyboard(CAMMOVenum direction, CAMSPDenum speed, 
		float delta_time);
	void ProcessMouseMovement(float x_offset, float y_offset, 
		GLboolean constrain_pitch = true);
	void ProcessMouseScroll(float y_offset);

private:
	static const float _FRUSTUM_NEAR_;
	static const float _FRUSTUM_FAR_;
	static const float _YAW_;
	static const float _PITCH_;
	static const float _SPEED_;
	static const float _SPEED_FAST_;
	static const float _SENSITIVITY_;
	static const float _FOV_;
	static const float _ASPECT_RATIO_;

	void updateCameraVectors();
};