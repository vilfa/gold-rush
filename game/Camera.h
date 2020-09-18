#ifndef CAMERA_H
#define CAMERA_H

#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Window.h"

enum CAMMOVenum {
	CAMERA_FORWARD,
	CAMERA_BACKWARD,
	CAMERA_LEFT,
	CAMERA_RIGHT
};

class Camera
{
public:
	glm::vec3 worldUp;
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;

	float frustumNear;
	float frustumFar;
	float yaw;
	float pitch;
	float movementSpeed;
	float mouseSensitivity;
	float fov;

	Camera(Window* window, float frustumNear = _FRUSTUM_NEAR, float frustumFar = _FRUSTUM_FAR, 
		glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), 
		float yaw = _YAW, float pitch = _PITCH);
	Camera(Window* window, float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch, 
		float frustumNear = _FRUSTUM_NEAR, float frustumFar = _FRUSTUM_FAR);

	glm::mat4 getViewMatrix() const;
	glm::mat4 getProjectionMatrix() const;
	void processKeyboard(CAMMOVenum direction, float deltaTime);
	void processMouseMovement(float xOffset, float yOffset, GLboolean constrainPitch = true);
	void processMouseScroll(float yOffset);

private:
	Window* window;
	static const float _FRUSTUM_NEAR;
	static const float _FRUSTUM_FAR;
	static const float _YAW;
	static const float _PITCH;
	static const float _SPEED;
	static const float _SENSITIVITY;
	static const float _FOV;

	void updateCameraVectors();
};

#endif // !CAMERA_H