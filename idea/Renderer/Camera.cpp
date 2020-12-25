#include "Renderer/Camera.h"

const float Camera::_FRUSTUM_NEAR_ = 0.1f;
const float Camera::_FRUSTUM_FAR_ = 100.0f;
const float Camera::_YAW_ = -90.0f;
const float Camera::_PITCH_ = 0.0f;
const float Camera::_SPEED_ = 3.5f;
const float Camera::_SPEED_FAST_ = 7.0f;
const float Camera::_SENSITIVITY_ = 0.08f;
const float Camera::_FOV_ = 45.0f;
const float Camera::_ASPECT_RATIO_ = 16.0f / 9.0f;

Camera::Camera(glm::vec3 position,
	float aspect_ratio,
	glm::vec3 up,
	float frustum_near,
	float frustum_far,
	float yaw,
	float pitch) :
	position_(position),
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

glm::mat4 Camera::GetViewMatrix() const
{
	return glm::lookAt(position_, position_ + front_, up_);
}

glm::mat3 Camera::GetViewMatrix3() const
{
	return glm::mat3(glm::lookAt(position_, position_ + front_, up_));
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

void Camera::ProcessKeyboard(CAMMOVenum direction, CAMSPDenum speed,
	float delta_time)
{
	float velocity = (speed == CAMSPDenum::NORMAL) ? 
		movement_speed_ * delta_time : movement_speed_fast_ * delta_time;
	if (direction == CAMMOVenum::FORWARD)
	{
		position_ += front_ * velocity;
	}
	if (direction == CAMMOVenum::BACKWARD)
	{
		position_ -= front_ * velocity;
	}
	if (direction == CAMMOVenum::LEFT)
	{
		position_ -= right_ * velocity;
	}
	if (direction == CAMMOVenum::RIGHT)
	{
		position_ += right_ * velocity;
	}
}

void Camera::ProcessMouseMovement(float x_offset, float y_offset, 
	GLboolean constrain_pitch)
{
	x_offset *= mouse_sensitivity_;
	y_offset *= mouse_sensitivity_;

	yaw_ += x_offset;
	pitch_ += y_offset;

	if (constrain_pitch)
	{
		if (pitch_ > 89.0f)
		{
			pitch_ = 89.0f;
		}
		if (pitch_ < -89.0f)
		{
			pitch_ = -89.0f;
		}
	}

	updateCameraVectors();
}

void Camera::ProcessMouseScroll(float y_offset)
{
	fov_ -= y_offset;
	if (fov_ < 1.0f)
	{
		fov_ = 1.0f;
	}
	if (fov_ > 60.0f)
	{
		fov_ = 60.0f;
	}
}

/*
* Updates front, up and right camera vectors
*/
void Camera::updateCameraVectors()
{
	glm::vec3 front;
	front.x = cos(glm::radians(yaw_)) * cos(glm::radians(pitch_));
	front.y = sin(glm::radians(pitch_));
	front.z = sin(glm::radians(yaw_)) * cos(glm::radians(pitch_));

	/*
	* Recalculate the up and right vector.
	* Normalize all the vectors because their length gets closer to 0
	* the more you look up or down which results in slower movement.
	*/
	front_ = glm::normalize(front);
	right_ = glm::normalize(glm::cross(front_, world_up_));
	up_ = glm::normalize(glm::cross(right_, front_));
}
