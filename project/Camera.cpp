#include "Camera.h"


Camera::Camera(glm::vec3 position, glm::vec3 target, glm::vec3 worldup)
{
	Position = position;
	WorldUp = worldup;
	Forward = glm::normalize(target - position);
	Right = glm::normalize(glm::cross(Forward, WorldUp));
	Up = glm::normalize(glm::cross(Right, Forward));

	Pitch = 0.0;
	Yaw = -40.0;
	SpeedZ = 2;
	UpdateCameraVectors();
}

Camera::Camera(glm::vec3 position, float pitch, float yaw, glm::vec3 worldup)
{
	SpeedZ = 2;
	Position = position;
	WorldUp = worldup;
	Pitch = pitch;
	Yaw = yaw;
	Forward.x = glm::cos(Pitch)*glm::sin(Yaw);
	Forward.y = glm::sin(Pitch);		 
	Forward.z = glm::cos(Pitch)*glm::cos(Yaw);
	Right = glm::normalize(glm::cross(Forward, WorldUp));
	Up = glm::normalize(glm::cross(Right, Forward));
}

Camera::~Camera()
{
}

glm::mat4 Camera::GetViewMatrix()
{
	return glm::lookAt(Position, Position + Forward, WorldUp);
}

void Camera::ProcessMouseMovement(float deltaX, float deltaY)
{
	UpdateCameraVectors();
	Pitch -= deltaY;
	Yaw += deltaX;
	
}

void Camera::ForwardCameraPos()
{
	UpdateCameraVectors();
	Forward.y = glm::sin(0.0);
	Position += Forward*SpeedZ;
	
}
void Camera::BackCameraPos()
{
	UpdateCameraVectors();
	Forward.y = glm::sin(0.0);
	Position -= Forward * SpeedZ;
	
}
void Camera::UpdateCameraVectors()
{
	Forward.x = glm::cos(Pitch)*glm::sin(Yaw);
	Forward.y = glm::sin(Pitch);
	Forward.z = glm::cos(Pitch)*glm::cos(Yaw);
	Right = glm::normalize(glm::cross(Forward, WorldUp));
	Up = glm::normalize(glm::cross(Right, Forward));
	
}
