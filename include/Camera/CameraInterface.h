#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "./../../IO/IOTypes/IOTypeInterface.h"

#include <mutex>
#include <functional>

struct ICameraInfo {

	glm::vec3 m_CameraPos = glm::vec3(100, 10, 100);
	glm::vec3 m_CameraFront = glm::vec3(0, 0, -1);
	glm::vec3 m_CameraUp = glm::vec3(0, 1, 0);
	glm::vec3 m_CameraRight = glm::normalize(glm::cross(m_CameraFront, m_CameraUp));
	glm::mat4 view = glm::lookAt(m_CameraPos, m_CameraPos + m_CameraFront, m_CameraUp);
	glm::mat4 projection = glm::mat4(1.0f);

	glm::vec3 centralPoint = glm::vec3(0,0,0);

	float radius = 100;
	float m_Yaw = 0.1;
	float m_Pitch = 0.1;
	float fov = 60.0f;

	float nearPlane = 1.0f;
	float farPlane = 10000.0f;

	ICameraInfo() {

	}
};

class ICamera {
public:
	virtual ~ICamera() = default;
	virtual void update(int windowWidth, int windowHeight, double cursorX, double cursorY) = 0;
	ICameraInfo& getActiveCameraInfo() {
		std::lock_guard<std::mutex> lock(infoLock);
		return m_CameraInfo;
	}

	ICameraInfo& getProposedCameraInfo() {
		std::lock_guard<std::mutex> lock(infoLock);
		return m_ProposedCameraInfo;
	}

	void validateCameraInfo() {
		std::lock_guard<std::mutex> lock(infoLock);
		m_CameraInfo = m_ProposedCameraInfo;
	}

protected:
	ICameraInfo m_CameraInfo;
	ICameraInfo m_ProposedCameraInfo;

	std::mutex infoLock;
	bool isIOKeybindCallbackSet = false;
	IOInterface* m_CameraToIO;
};

enum CameraType{
	Orbital,
	FirstPerson,
	MaxNumberOfCameraTypes
};

struct ICameraControls {
	virtual ~ICameraControls() = default;


	//Assign KeyBinds
	virtual void bindKeys() = 0;
	//Remove KeyBinds
	virtual void unbindKeys() = 0;

	//Render ImGui controls for variable controls
	virtual void renderGuiControls() = 0;
};