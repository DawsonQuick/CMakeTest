#pragma once
#include "./../CameraInterface.h"
#include <iostream>

class FirstPersonCamera : public ICamera {
public:
	FirstPersonCamera(IOInterface* cameraToIO);
	void update(int windowWidth, int windowHeight, double cursorX, double cursorY) override;

private:
	bool m_FirstMouse, m_trackingEnabled;
	int m_WindowWidth;
	int m_WindowHeight;
	double m_LastX;
	double m_LastY;
	glm::vec3 cameraSpeed = glm::vec3(1.0);
	double m_Xoffset;
	double m_Yoffset;

	bool firstUpdate;
	bool mouseControlEnabled;


	void updateCursorPosition();
	void updateCameraVectors();

	void mouse_Dragcallback();
	void camera_MoveFoward();
	void camera_MoveBackwards();
	void camera_MoveLeft();
	void camera_MoveRight();
	void camera_MoveUp();
	void camera_MoveDown();

	void toggleMouseControl();

	friend struct FirstPersonControls;
};

struct FirstPersonControls : public ICameraControls {


	/*
	* TODO: Add keybinds for operating camera
	*
	* Default Right Mouse (Hold): used to rotate the camera around the origin of the radius
	* Default Left Mouse (Press): use to cast a ray out from the camera position into the world (world interaction)
	*
	*/



	/*
	* Variables:
	*
	* float controls_CameraRadius
	*
	*/

	FirstPersonCamera* camera;  // Pointer to the camera instance

	FirstPersonControls(FirstPersonCamera* cam) : camera(cam) {
	}

	void bindKeys() override {
		camera->m_CameraToIO->addKeyToCheckFor("Camera Move Foward",
			{ IO_W },
			[this]() { camera->camera_MoveFoward(); },
			OnHold, KeyboardInput, true
		);

		camera->m_CameraToIO->addKeyToCheckFor("Camera Move Backwards",
			{ IO_S },
			[this]() { camera->camera_MoveBackwards(); },
			OnHold, KeyboardInput, true
		);

		camera->m_CameraToIO->addKeyToCheckFor("Camera Move Left",
			{ IO_A },
			[this]() { camera->camera_MoveLeft(); },
			OnHold, KeyboardInput, true
		);

		camera->m_CameraToIO->addKeyToCheckFor("Camera Move Right",
			{ IO_D },
			[this]() { camera->camera_MoveRight(); },
			OnHold, KeyboardInput, true
		);

		camera->m_CameraToIO->addKeyToCheckFor("Camera Move Up",
			{ IO_SPACE },
			[this]() { camera->camera_MoveUp(); },
			OnHold, KeyboardInput, true
		);

		camera->m_CameraToIO->addKeyToCheckFor("Camera Move Down",
			{ IO_LEFT_SHIFT },
			[this]() { camera->camera_MoveDown(); },
			OnHold, KeyboardInput, true
		);

		camera->m_CameraToIO->addKeyToCheckFor("Toggle Free Camera",
			{ IO_ESCAPE },
			[this]() { camera->toggleMouseControl(); },
			OnPress, KeyboardInput, true
		);


	}

	void unbindKeys() override {

	}

	void renderGuiControls() override {

	}

};