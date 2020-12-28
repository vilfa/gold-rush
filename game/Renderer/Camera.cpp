#include "Renderer/Camera.h"

const float Camera::_FRUSTUM_NEAR_ = 0.1f;
const float Camera::_FRUSTUM_FAR_ = 100.0f;
const float Camera::_YAW_ = 0.0f;
const float Camera::_PITCH_ = 0.0f;
const float Camera::_SPEED_ = 3.5f;
const float Camera::_SPEED_FAST_ = 7.0f;
const float Camera::_SENSITIVITY_ = 0.08f;
const float Camera::_FOV_ = 50.0f;
const float Camera::_ASPECT_RATIO_ = 16.0f / 9.0f;
const glm::vec3 Camera::_DEFAULT_PLAYER_OFFSET_ = glm::vec3(0.0f, 2.0f, 3.0f);

Camera::Camera(glm::vec3 position,
	glm::vec3 follow_player_offset,
	float aspect_ratio,
	glm::vec3 up,
	float frustum_near,
	float frustum_far,
	float yaw,
	float pitch) :
	position_(position),
	player_position_(glm::vec3(0.0f, 0.0f, 0.0f)),
	player_offset_(follow_player_offset),
	player_radius_(3.0f),
	world_up_(up),
	_aspect_ratio_(aspect_ratio),
	frustum_near_(frustum_near),
	frustum_far_(frustum_far),
	yaw_(yaw),
	pitch_(pitch),
	front_(glm::vec3(0.0f, 0.0f, -1.0f)),
	movement_speed_(Camera::_SPEED_),
	movement_speed_fast_(Camera::_SPEED_FAST_),
	mouse_sensitivity_(Camera::_SENSITIVITY_),
	fov_(Camera::_FOV_)
{
	position_ += follow_player_offset;
	updateCameraVectors();
}

Camera::Camera(float pos_x,
	float pos_y,
	float pos_z,
	float up_x,
	float up_y,
	float up_z,
	float yaw,
	float pitch,
	float aspect_ratio,
	float frustum_near,
	float frustum_far) :
	position_(glm::vec3(pos_x, pos_y, pos_z)),
	world_up_(glm::vec3(up_x, up_y, up_z)),
	_aspect_ratio_(aspect_ratio),
	frustum_near_(frustum_near),
	frustum_far_(frustum_far),
	yaw_(yaw),
	pitch_(pitch),
	front_(glm::vec3(0.0f, 0.0f, -1.0f)),
	movement_speed_(Camera::_SPEED_),
	movement_speed_fast_(Camera::_SPEED_FAST_),
	mouse_sensitivity_(Camera::_SENSITIVITY_),
	fov_(Camera::_FOV_)
{
	updateCameraVectors();
}

void Camera::SetPlayerPosition(glm::vec3 player_pos)
{
	player_position_ = player_pos;
}

glm::mat4 Camera::GetViewMatrix() const
{
	glm::vec3 up_off(0.0f, 1.5f, 0.0f);
	return glm::lookAt(position_, player_position_ + up_off, up_);
}

glm::mat3 Camera::GetViewMatrix3() const
{
	glm::vec3 up_off(0.0f, 1.5f, 0.0f);
	return glm::mat3(glm::lookAt(position_, player_position_ + up_off, up_));
}

glm::mat4 Camera::GetProjectionMatrix() const
{
	return glm::perspective(
		glm::radians(fov_),
		_aspect_ratio_,
		frustum_near_,
		frustum_far_
	);
}

glm::mat4 Camera::GetProjectionViewMatrix() const
{
	return GetProjectionMatrix() * GetViewMatrix();
}

void Camera::HandleMouse(float x_offset, float y_offset)
{
	x_offset *= mouse_sensitivity_;
	y_offset *= mouse_sensitivity_;

	yaw_ -= x_offset;
	//pitch_ += y_offset;

	if (pitch_ > 89.0f)
	{
		pitch_ = 89.0f;
	}
	if (pitch_ < -89.0f)
	{
		pitch_ = -89.0f;
	}

	updateCameraVectors();
}

void Camera::FollowPlayer()
{
	player_offset_.x = sin(glm::radians(yaw_)) * player_radius_;
	player_offset_.y = 2.0f;
	player_offset_.z = cos(glm::radians(yaw_)) * player_radius_;

	position_ = player_position_ + player_offset_;
}

void Camera::updateCameraVectors()
{
	glm::vec3 front = player_position_ - position_;
	front.y = 0.0f;
	
	// Recalculate the up and right vector.
	// Normalize all the vectors because their length gets closer to 0
	// the more you look up or down which results in slower movement.
	//
	front_ = glm::normalize(front);
	right_ = glm::normalize(glm::cross(front_, world_up_));
	up_ = glm::normalize(glm::cross(right_, front_));
}
