#include "Camera/OrbitalCamera/OrbitalCamera.h"

OrbitalCamera::OrbitalCamera(IOInterface* cameraToIO)
	: m_LastX(0.0), m_LastY(0.0), firstUpdate(false)
{
	m_CameraToIO = std::move(cameraToIO);
	updateCameraVectors();
}

void OrbitalCamera::update(int windowWidth, int windowHeight, double cursorX, double cursorY) {

	m_WindowWidth = windowWidth;
	m_WindowHeight = windowHeight;

	//If it is the first update for the camera , calculate using default values and dont consider changes
	if (!firstUpdate) {
		mouse_Dragcallback();
		firstUpdate = true;
		return;
	}

	m_Xoffset = cursorX - m_LastX;
	m_Yoffset = cursorY - m_LastY;

	m_LastX = cursorX;
	m_LastY = cursorY;


}

void OrbitalCamera::mouse_Dragcallback() {

	m_ProposedCameraInfo.m_Yaw += m_Xoffset * 0.1;
	m_ProposedCameraInfo.m_Pitch += m_Yoffset * 0.1;


	if (m_ProposedCameraInfo.m_Pitch > 89.0f)  m_ProposedCameraInfo.m_Pitch = 89.0f;
	if (m_ProposedCameraInfo.m_Pitch < -89.0f) m_ProposedCameraInfo.m_Pitch = -89.0f;

	updateCameraVectors();
}

void OrbitalCamera::camera_MoveFoward() {
	glm::vec3 forward = glm::normalize(glm::vec3(m_ProposedCameraInfo.m_CameraFront.x, 0, m_ProposedCameraInfo.m_CameraFront.z));
	glm::vec3 movement = forward * cameraSpeed;
	m_ProposedCameraInfo.centralPoint += movement;
	m_ProposedCameraInfo.m_CameraPos += movement;
	updateCameraVectors();
}
void OrbitalCamera::camera_MoveBackwards() {
	glm::vec3 backward = glm::normalize(glm::vec3(m_ProposedCameraInfo.m_CameraFront.x, 0, m_ProposedCameraInfo.m_CameraFront.z));
	glm::vec3 movement = -backward * cameraSpeed;

	m_ProposedCameraInfo.centralPoint += movement;
	m_ProposedCameraInfo.m_CameraPos += movement;
	updateCameraVectors();
}
void OrbitalCamera::camera_MoveLeft() {
	glm::vec3 left = glm::normalize(glm::vec3(m_ProposedCameraInfo.m_CameraRight.x, 0, m_ProposedCameraInfo.m_CameraRight.z));
	glm::vec3 movement = -left * cameraSpeed;

	m_ProposedCameraInfo.centralPoint += movement;
	m_ProposedCameraInfo.m_CameraPos += movement;
	updateCameraVectors();
}
void OrbitalCamera::camera_MoveRight() {
	glm::vec3 right = glm::normalize(glm::vec3(m_ProposedCameraInfo.m_CameraRight.x, 0, m_ProposedCameraInfo.m_CameraRight.z));
	glm::vec3 movement = right * cameraSpeed;

	m_ProposedCameraInfo.centralPoint += movement;
	m_ProposedCameraInfo.m_CameraPos += movement;
	updateCameraVectors();
}

void OrbitalCamera::mouseScrollWheelcallback(double xOffset, double yOffset) {
	std::cout << "Camera received scroll wheel callback information: " << xOffset << " - " << yOffset << std::endl;
	m_ProposedCameraInfo.radius += yOffset * 50;
	updateCameraVectors();
}

void OrbitalCamera::updateCameraVectors() {

	glm::vec3 position;
	position.x = m_ProposedCameraInfo.centralPoint.x + m_ProposedCameraInfo.radius * cos(glm::radians(m_ProposedCameraInfo.m_Yaw)) * cos(glm::radians(m_ProposedCameraInfo.m_Pitch));
	position.y = m_ProposedCameraInfo.centralPoint.y + m_ProposedCameraInfo.radius * sin(glm::radians(m_ProposedCameraInfo.m_Pitch));
	position.z = m_ProposedCameraInfo.centralPoint.z + m_ProposedCameraInfo.radius * sin(glm::radians(m_ProposedCameraInfo.m_Yaw)) * cos(glm::radians(m_ProposedCameraInfo.m_Pitch));

	m_ProposedCameraInfo.m_CameraPos = position;
	// Camera front vector always looks at the central point
	m_ProposedCameraInfo.m_CameraFront = glm::normalize(m_ProposedCameraInfo.centralPoint - position);

	m_ProposedCameraInfo.m_CameraRight = glm::normalize(glm::cross(m_ProposedCameraInfo.m_CameraFront, glm::vec3(0.0f, 1.0f, 0.0f)));
	m_ProposedCameraInfo.m_CameraUp = glm::normalize(glm::cross(m_ProposedCameraInfo.m_CameraRight, m_ProposedCameraInfo.m_CameraFront));

	m_ProposedCameraInfo.view = glm::lookAt(position, m_ProposedCameraInfo.centralPoint, m_ProposedCameraInfo.m_CameraUp);
	m_ProposedCameraInfo.projection = glm::perspective(glm::radians(m_ProposedCameraInfo.fov), (float)m_WindowWidth / (float)m_WindowHeight, m_ProposedCameraInfo.nearPlane, m_ProposedCameraInfo.farPlane);

}