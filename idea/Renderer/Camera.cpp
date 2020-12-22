#include "Renderer/Camera.h"

const float Camera::_FRUSTUM_NEAR = 0.1f;
const float Camera::_FRUSTUM_FAR = 100.0f;
const float Camera::_YAW = -90.0f;
const float Camera::_PITCH = 0.0f;
const float Camera::_SPEED = 3.5f;
const float Camera::_SPEED_FAST = 7.0f;
const float Camera::_SENSITIVITY = 0.08f;
const float Camera::_FOV = 45.0f;

Camera::Camera(
	Window& window, 
	glm::vec3 position, 
	glm::vec3 up,
	float frustumNear, 
	float frustumFar,
	float yaw, 
	float pitch
) :
	window(window),
	Position(position),
	WorldUp(up),
	FrustumNear(frustumNear),
	FrustumFar(frustumFar),
	Yaw(yaw),
	Pitch(pitch),
	Front(glm::vec3(0.0f, 0.0f, -1.0f)), 
	MovementSpeed(Camera::_SPEED), 
	MovementSpeedFast(Camera::_SPEED_FAST), 
	MouseSensitivity(Camera::_SENSITIVITY), 
	Fov(Camera::_FOV)
{
	updateCameraVectors();
}

Camera::Camera(
	Window& window, 
	float posX, 
	float posY, 
	float posZ, 
	float upX, 
	float upY, 
	float upZ, 
	float yaw, 
	float pitch, 
	float frustumNear, 
	float frustumFar
) : 
	window(window),
	Position(glm::vec3(posX, posY, posZ)),
	WorldUp(glm::vec3(upX, upY, upZ)),
	FrustumNear(frustumNear),
	FrustumFar(frustumFar),
	Yaw(yaw),
	Pitch(pitch),
	Front(glm::vec3(0.0f, 0.0f, -1.0f)),
	MovementSpeed(Camera::_SPEED), 
	MovementSpeedFast(Camera::_SPEED_FAST), 
	MouseSensitivity(Camera::_SENSITIVITY), 
	Fov(Camera::_FOV)
{
	updateCameraVectors();
}

glm::mat4 Camera::GetViewMatrix() const
{
	return glm::lookAt(Position, Position + Front, Up);
}

glm::mat4 Camera::GetProjectionMatrix() const
{
	return glm::perspective(
		glm::radians(Fov),
		(float)window.GetWidth() / (float)window.GetHeight(),
		FrustumNear,
		FrustumFar
	);
}

glm::mat4 Camera::GetProjectionViewMatrix() const
{
	return GetProjectionMatrix() * GetViewMatrix();
}

void Camera::ProcessKeyboard(
	CAMMOVenum direction, 
	CAMSPDenum speed, 
	float deltaTime
)
{
	float velocity = (speed == CAMSPDenum::NORMAL) ? 
		MovementSpeed * deltaTime : MovementSpeedFast * deltaTime;
	if (direction == CAMMOVenum::FORWARD)
	{
		Position += Front * velocity;
	}
	if (direction == CAMMOVenum::BACKWARD)
	{
		Position -= Front * velocity;
	}
	if (direction == CAMMOVenum::LEFT)
	{
		Position -= Right * velocity;
	}
	if (direction == CAMMOVenum::RIGHT)
	{
		Position += Right * velocity;
	}
}

void Camera::ProcessMouseMovement(
	float xOffset, 
	float yOffset, 
	GLboolean constrainPitch
)
{
	xOffset *= MouseSensitivity;
	yOffset *= MouseSensitivity;

	Yaw += xOffset;
	Pitch += yOffset;

	if (constrainPitch)
	{
		if (Pitch > 89.0f)
		{
			Pitch = 89.0f;
		}
		if (Pitch < -89.0f)
		{
			Pitch = -89.0f;
		}
	}

	updateCameraVectors();
}

void Camera::ProcessMouseScroll(float yOffset)
{
	Fov -= yOffset;
	if (Fov < 1.0f)
	{
		Fov = 1.0f;
	}
	if (Fov > 60.0f)
	{
		Fov = 60.0f;
	}
}

/*
* Updates front, up and right camera vectors
*/
void Camera::updateCameraVectors()
{
	glm::vec3 front;
	front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	front.y = sin(glm::radians(Pitch));
	front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));

	/*
	* Recalculate the up and right vector.
	* Normalize all the vectors because their length gets closer to 0
	* the more you look up or down which results in slower movement.
	*/
	Front = glm::normalize(front);
	Right = glm::normalize(glm::cross(Front, WorldUp));
	Up = glm::normalize(glm::cross(Right, Front));
}
