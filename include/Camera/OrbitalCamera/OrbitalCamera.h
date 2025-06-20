#pragma once
#include <iostream>
#include "./../CameraInterface.h"

class OrbitalCamera : public ICamera {
public:
	OrbitalCamera(IOInterface* cameraToIO);
	void update(int windowWidth, int windowHeight, double cursorX, double cursorY) override;
private:
	int m_WindowWidth;
	int m_WindowHeight;
	double m_LastX;
	double m_LastY;
	glm::vec3 cameraSpeed = glm::vec3(3);
	double m_Xoffset = 0.0;
	double m_Yoffset = 0.0;

	bool firstUpdate;

	void mouse_Dragcallback();
	void mouseScrollWheelcallback(double xOffset, double yOffset);
	void updateCameraVectors();

	void camera_MoveFoward();
	void camera_MoveBackwards();
	void camera_MoveLeft();
	void camera_MoveRight();
	void camera_MoveUp();
	void camera_MoveDown();

	friend struct OribitalControls;
};

struct OribitalControls : public ICameraControls {


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

	OrbitalCamera* camera;  // Pointer to the camera instance

	OribitalControls(OrbitalCamera* cam) : camera(cam) {
	}

	void bindKeys() override {
		camera->m_CameraToIO->addKeyToCheckFor("Camera Move Foward",
			{ IO_W },
			[this]() { camera->camera_MoveFoward(); },
			OnHold, KeyboardInput, false
		);

		camera->m_CameraToIO->addKeyToCheckFor("Camera Move Backwards",
			{ IO_S },
			[this]() { camera->camera_MoveBackwards(); },
			OnHold, KeyboardInput, false
		);

		camera->m_CameraToIO->addKeyToCheckFor("Camera Move Left",
			{ IO_A },
			[this]() { camera->camera_MoveLeft(); },
			OnHold, KeyboardInput, false
		);

		camera->m_CameraToIO->addKeyToCheckFor("Camera Move Right",
			{ IO_D },
			[this]() { camera->camera_MoveRight(); },
			OnHold, KeyboardInput, false
		);


		camera->m_CameraToIO->addKeyToCheckFor("Drag Camera",
			{ IO_MOUSE_BUTTON_2 },
			[this]() { camera->mouse_Dragcallback(); },
			OnHold, MouseButtonInput, false
		);

		camera->m_CameraToIO->addMouseScrollWheelCheck("Orbital Cam Scroll Wheel",
			[this](double x, double y) { camera->mouseScrollWheelcallback(x,y); });

	}

	void unbindKeys() override {

	}

	void renderGuiControls() override {

	}

};