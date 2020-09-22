#ifndef CAMERA_H
#define CAMERA_H

#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Window.h"

enum class CAMMOVenum {
	CAMERA_FORWARD,
	CAMERA_BACKWARD,
	CAMERA_LEFT,
	CAMERA_RIGHT
};

enum class CAMSPDenum {
	CAMERA_NORMAL,
	CAMERA_FAST
};

class Camera
{
public:
	glm::vec3 WorldUp;
	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;

	float FrustumNear;
	float FrustumFar;
	float Yaw;
	float Pitch;
	float MovementSpeed;
	float MovementSpeedFast;
	float MouseSensitivity;
	float Fov;

	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
		float frustumNear = _FRUSTUM_NEAR, float frustumFar = _FRUSTUM_FAR, 
		float yaw = _YAW, float pitch = _PITCH);
	Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch, 
		float frustumNear = _FRUSTUM_NEAR, float frustumFar = _FRUSTUM_FAR);

	glm::mat4 GetViewMatrix() const;
	void ProcessKeyboard(CAMMOVenum direction, CAMSPDenum speed, float deltaTime);
	void ProcessMouseMovement(float xOffset, float yOffset, GLboolean constrainPitch = true);
	void ProcessMouseScroll(float yOffset);

private:
	Window* window;
	static const float _FRUSTUM_NEAR;
	static const float _FRUSTUM_FAR;
	static const float _YAW;
	static const float _PITCH;
	static const float _SPEED;
	static const float _SPEED_FAST;
	static const float _SENSITIVITY;
	static const float _FOV;

	void updateCameraVectors();
};

#endif // !CAMERA_H