#pragma once
#include <memory>
#include <iostream>
#include "./GLFW/GLFW_IO.h"

inline std::unique_ptr<IOInterface> generateIO(IOType type) {
	switch (type) {
	case IOType::GLFW:
		return std::make_unique<GLFW_IO>();
		break;
	default:
		std::cerr << "Unsupported IO Type" << std::endl;
	}
}