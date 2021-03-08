#include "Camera.h"

Camera::Camera() {}

Camera::Camera(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch, GLfloat startMoveSpeed, GLfloat startTurnSpeed)
{
	position = startPosition;
	worldUp = startUp;
	yaw = startYaw;
	pitch = startPitch;
	front = glm::vec3(0.0f, 0.0f, -1.0f);

	moveSpeed = startMoveSpeed;
	turnSpeed = startTurnSpeed;

	update();
}

bool Camera::keyControl(bool* keys, GLfloat deltaTime) {
	GLfloat velocity = moveSpeed * deltaTime;
	bool keyChanged = false;

	if (keys[GLFW_KEY_W]) {
		position += front * velocity;
		keyChanged = true;
	}

	if (keys[GLFW_KEY_S]) {
		position -= front * velocity;
		keyChanged = true;
	}

	if (keys[GLFW_KEY_A]) {
		position -= right * velocity;
		keyChanged = true;
	}

	if (keys[GLFW_KEY_D]) {
		position += right * velocity;
		keyChanged = true;
	}

	if (keys[GLFW_KEY_R]) {
		position = glm::vec3(0.0f, 0.0f, 0.0f);
		keyChanged = true;
	}

	return keyChanged;
}

bool Camera::mouseControl(GLfloat xChange, GLfloat yChange) {
	xChange *= turnSpeed;
	yChange *= turnSpeed;

	bool mouseChanged = (xChange != 0) || (yChange != 0);

	yaw += xChange;
	pitch += yChange;

	if (pitch > 89.0f) {
		pitch = 89.0f;
	}

	if (pitch < -89.0f) {
		pitch = -89.0f;
	}

	update();

	return mouseChanged;
}

glm::mat4 Camera::calculateViewMatrix() {
	return glm::lookAt(position, position + front, up);
}

glm::vec3 Camera::getCameraPosition() {
	return position;
}

void Camera::update() {
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	front = glm::normalize(front);

	right = glm::normalize(glm::cross(front, worldUp));
	up = glm::normalize(glm::cross(right, front));
}

Camera::~Camera()
{
}
