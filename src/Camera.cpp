#include "Camera.h"

void Camera::Init(const glm::vec3& up, float speed)
{
	worldUp = up;
	camSpeed = speed;
}

void Camera::UpdateCamera()
{
	updateCameraVectors();
	m_view = glm::lookAt(camPosition, camTarget, worldUp);
}

void Camera::updateCameraVectors()
{
	camFront = camPosition - camTarget;
	camRight = glm::normalize(glm::cross(worldUp, camFront));
	camUp = glm::cross(camFront, camRight);
}

void Camera::SetPose(const glm::vec3& position, const glm::vec3 target)
{
	SetCameraPosition(position);
	SetCameraTarget(target);
}

//pitch:x->y  yaw:x->z
void Camera::SetPose(const glm::vec3& position, const float pitch, const float yaw, const float roll)
{
	SetCameraPosition(position);

	glm::vec3 direction = glm::vec3(1.0);
	float rYaw = cos(glm::radians(pitch));
	direction.x = rYaw * cos(glm::radians(yaw));
	direction.y = sin(glm::radians(pitch));
	direction.z = rYaw * sin(glm::radians(yaw));

	SetCameraTarget(position + direction);
}