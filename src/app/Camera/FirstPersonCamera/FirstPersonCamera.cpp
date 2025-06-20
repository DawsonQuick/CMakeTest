#include "Camera/FirstPersonCamera/FirstPersonCamera.h"

FirstPersonCamera::FirstPersonCamera(IOInterface* cameraToIO)
{
    m_CameraToIO = std::move(cameraToIO);
    updateCameraVectors();
}

void FirstPersonCamera::update(int windowWidth, int windowHeight, double cursorX, double cursorY) {
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

    if (!mouseControlEnabled) {
        mouse_Dragcallback();
    }
}

void FirstPersonCamera::updateCursorPosition() {
    if (mouseControlEnabled) {
        m_CameraToIO->setCursorPosition(m_WindowWidth / 2.0, m_WindowHeight / 2.0);
    }
}

void FirstPersonCamera::mouse_Dragcallback() {

    updateCursorPosition(); // Keep the mouse at the center of the screen, even though it should be hidden


    m_ProposedCameraInfo.m_Yaw += m_Xoffset * 0.1;
    m_ProposedCameraInfo.m_Pitch -= m_Yoffset * 0.1;


    if (m_ProposedCameraInfo.m_Pitch > 89.0f)  m_ProposedCameraInfo.m_Pitch = 89.0f;
    if (m_ProposedCameraInfo.m_Pitch < -89.0f) m_ProposedCameraInfo.m_Pitch = -89.0f;

    updateCameraVectors();
}


void FirstPersonCamera::updateCameraVectors() {
    // Calculate the new front vector
    glm::vec3 front;
    front.x = cos(glm::radians(m_ProposedCameraInfo.m_Yaw)) * cos(glm::radians(m_ProposedCameraInfo.m_Pitch));
    front.y = sin(glm::radians(m_ProposedCameraInfo.m_Pitch));
    front.z = sin(glm::radians(m_ProposedCameraInfo.m_Yaw)) * cos(glm::radians(m_ProposedCameraInfo.m_Pitch));
    m_ProposedCameraInfo.m_CameraFront = glm::normalize(front);  // Normalize the front vector

    // Calculate right and up vectors based on the front vector
    m_ProposedCameraInfo.m_CameraRight = glm::normalize(glm::cross(m_ProposedCameraInfo.m_CameraFront, glm::vec3(0.0f, 1.0f, 0.0f))); // Right vector
    m_ProposedCameraInfo.m_CameraUp = glm::normalize(glm::cross(m_ProposedCameraInfo.m_CameraRight, m_ProposedCameraInfo.m_CameraFront)); // Up vector

    // View matrix
    m_ProposedCameraInfo.view = glm::lookAt(m_ProposedCameraInfo.m_CameraPos, m_ProposedCameraInfo.m_CameraPos + m_ProposedCameraInfo.m_CameraFront, m_ProposedCameraInfo.m_CameraUp);

    float aspect = (float)m_WindowWidth / (float)m_WindowHeight;
    if (m_WindowHeight == 0 || fabs(aspect) < std::numeric_limits<float>::epsilon()) {
    	// Prevent division by zero or degenerate aspect ratio
    	aspect = 1.0f; // Fallback to square viewport
    }

    m_ProposedCameraInfo.projection = glm::perspective(
    	glm::radians(m_ProposedCameraInfo.fov),
    	aspect,
    	m_ProposedCameraInfo.nearPlane,
    	m_ProposedCameraInfo.farPlane
    );
}

void FirstPersonCamera::camera_MoveFoward() {
    if (!mouseControlEnabled) {
        m_ProposedCameraInfo.m_CameraPos += cameraSpeed * m_ProposedCameraInfo.m_CameraFront;
        updateCameraVectors();
    }
}
void FirstPersonCamera::camera_MoveBackwards() {
    if (!mouseControlEnabled) {
        m_ProposedCameraInfo.m_CameraPos -= cameraSpeed * m_ProposedCameraInfo.m_CameraFront;
        updateCameraVectors();
    }
}
void FirstPersonCamera::camera_MoveLeft() {
    if (!mouseControlEnabled) {
        m_ProposedCameraInfo.m_CameraPos -= cameraSpeed * m_ProposedCameraInfo.m_CameraRight;
        updateCameraVectors();
    }
}
void FirstPersonCamera::camera_MoveRight() {
    if (!mouseControlEnabled) {
        m_ProposedCameraInfo.m_CameraPos += cameraSpeed * m_ProposedCameraInfo.m_CameraRight;
        updateCameraVectors();
    }
}
void FirstPersonCamera::camera_MoveUp() {
    if (!mouseControlEnabled) {
        m_ProposedCameraInfo.m_CameraPos += cameraSpeed * m_ProposedCameraInfo.m_CameraUp;
        updateCameraVectors();
    }
}
void FirstPersonCamera::camera_MoveDown() {
    if (!mouseControlEnabled) {
        m_ProposedCameraInfo.m_CameraPos -= cameraSpeed * m_ProposedCameraInfo.m_CameraUp;
        updateCameraVectors();
    }
}

void FirstPersonCamera::toggleMouseControl() {
    mouseControlEnabled = !mouseControlEnabled;
    
    if (!mouseControlEnabled) {

        //Set the cursor location to the last location is was set to before disabling tracking
        m_CameraToIO->setCursorPosition(m_LastX, m_LastY);

        // Hide the mouse and lock it to the window
        m_CameraToIO->setInputMode(CURSOR_DISABLE);
    }
    else {
        // Make the mouse visible and allow it to move freely again
        m_CameraToIO->setInputMode(CURSOR_NORMAL);

        // Set the last known mouse position to the center of the screen
        m_LastX = m_WindowWidth / 2.0f;
        m_LastY = m_WindowHeight / 2.0f;

        // Set the actual cursor position to the center of the screen
        m_CameraToIO->setCursorPosition(m_LastX, m_LastY);

    }
    
}
