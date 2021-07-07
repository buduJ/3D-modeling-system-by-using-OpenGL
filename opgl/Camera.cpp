#include "Camera.h"


Camera::Camera(glm::vec3 position, glm::vec3 target, glm::vec3 worldup)
{
	Position = position;
	WorldUp = worldup;
	Forward = glm::normalize(target - position);
	Right = glm::normalize(glm::cross(Forward, WorldUp));
	Up = glm::normalize( glm::cross(Right, Forward));

}

Camera::Camera(glm::vec3 position, float pitch, float yaw,glm::vec3 worldup) 
{
	Position = position;
	WorldUp = worldup;
	Pitch = pitch;
	Yaw = yaw;
	Forward.x = glm::cos(glm::radians(Pitch)) * glm::sin(glm::radians(Yaw)); // 译注：direction代表摄像机的前轴(Front)，这个前轴是和本文第一幅图片的第二个摄像机的方向向量是相反的
	Forward.y = glm::sin(glm::radians(Pitch));
	Forward.z = glm::cos(glm::radians(Pitch)) * glm::cos(glm::radians(Yaw));
	Right = glm::normalize(glm::cross(Forward, WorldUp));
	Up = glm::normalize(glm::cross(Right, Forward));

}

Camera::~Camera()
{

}

glm::mat4 Camera::GetViewMatrix() {
	return glm::lookAt(Position, Position + Forward, WorldUp);
}

void Camera::UpdateCameraVectors() {
	Forward.x = glm::cos(glm::radians(Pitch)) * glm::sin(glm::radians(Yaw)); // 译注：direction代表摄像机的前轴(Front)，这个前轴是和本文第一幅图片的第二个摄像机的方向向量是相反的
	Forward.y = glm::sin(glm::radians(Pitch));
	Forward.z = glm::cos(glm::radians(Pitch)) * glm::cos(glm::radians(Yaw));
	Right = glm::normalize(glm::cross(Forward, WorldUp));
	Up = glm::normalize(glm::cross(Right, Forward));
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset) {
	Pitch += yoffset;
	if (Pitch > 89.0f)
		Pitch = 89.0f;
	if (Pitch < -89.0f)
		Pitch = -89.0f;
	Yaw -= xoffset;
	UpdateCameraVectors();
}

void Camera::UpdateCameraPos() {
	Position += Forward * speedZ * 0.005f + Right * speedX * 0.005f + Up * speedY * 0.005f;
}

