#include "Camera.h"

const float Camera::_YAW = -90.0f;
const float Camera::_PITCH = 0.0f;
const float Camera::_SPEED = 2.5f;
const float Camera::_SENSITIVITY = 0.08f;
const float Camera::_FOV = 45.0f;

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch) : front(glm::vec3(0.0f, 0.0f, -1.0f)), movementSpeed(Camera::_SPEED), mouseSensitivity(Camera::_SENSITIVITY), fov(Camera::_FOV)
{
	this->position = position;
	this->worldUp = up;
	this->yaw = yaw;
	this->pitch = pitch;
	updateCameraVectors();
}

Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : front(glm::vec3(0.0f, 0.0f, -1.0f)), movementSpeed(Camera::_SPEED), mouseSensitivity(Camera::_SENSITIVITY), fov(Camera::_FOV)
{
	this->position = glm::vec3(posX, posY, posZ);
	this->worldUp = glm::vec3(upX, upY, upZ);
	this->yaw = yaw;
	this->pitch = pitch;
	updateCameraVectors();
}

glm::mat4 Camera::getViewMatrix() const
{
	return glm::lookAt(position, position + front, up);
}

void Camera::processKeyboard(CAMMOVenum direction, float deltaTime)
{
	float velocity = movementSpeed * deltaTime;
	if (direction == CAMERA_FORWARD)
	{
		position += front * velocity;
	}
	if (direction == CAMERA_BACKWARD)
	{
		position -= front * velocity;
	}
	if (direction == CAMERA_LEFT)
	{
		position -= right * velocity;
	}
	if (direction == CAMERA_RIGHT)
	{
		position += right * velocity;
	}
}

void Camera::processMouseMovement(float xOffset, float yOffset, GLboolean constrainPitch)
{
	xOffset *= mouseSensitivity;
	yOffset *= mouseSensitivity;

	yaw += xOffset;
	pitch += yOffset;

	if (constrainPitch)
	{
		if (pitch > 89.0f)
		{
			pitch = 89.0f;
		}
		if (pitch < -89.0f)
		{
			pitch = -89.0f;
		}
	}

	updateCameraVectors();
}

void Camera::processMouseScroll(float yOffset)
{
	fov -= yOffset;
	if (fov < 1.0f)
	{
		fov = 1.0f;
	}
	if (fov > 60.0f)
	{
		fov = 60.0f;
	}
}

/*
* Updates front, up and right camera vectors
*/
void Camera::updateCameraVectors()
{
	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw) * cos(glm::radians(pitch)));

	/*
	* Recalculate the up and right vector.
	* Normalize all the vectors because their length gets closer to 0
	* the more you look up or down which results in slower movement.
	*/
	front = glm::normalize(front);
	right = glm::normalize(glm::cross(front, worldUp));
	up = glm::normalize(glm::cross(right, front));
}
