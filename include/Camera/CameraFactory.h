#pragma once
#include "OrbitalCamera/OrbitalCamera.h"
#include "FirstPersonCamera/FirstPersonCamera.h"

#include <memory>
#include <iostream>


inline std::unique_ptr<ICamera> createCameraObject(CameraType type , 
	IOInterface* cameraToIO) {
	switch (type) {
	case CameraType::Orbital:
		return std::make_unique<OrbitalCamera>(cameraToIO);
	case CameraType::FirstPerson:
		return std::make_unique<FirstPersonCamera>(cameraToIO);
	default:
		std::cerr << "Unsupported Camera Type" << std::endl;
	}
}

inline std::unique_ptr<ICameraControls> createCameraControls(CameraType type, ICamera* camera) {
	switch (type) {
		case CameraType::Orbital:
			return std::make_unique<OribitalControls>(static_cast<OrbitalCamera*>(camera));
		case CameraType::FirstPerson:
			return std::make_unique<FirstPersonControls>(static_cast<FirstPersonCamera*>(camera));
		default:
			std::cerr << "Unsupported Camera Control Type" << std::endl;
	}
}

