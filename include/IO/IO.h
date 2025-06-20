#pragma once
#include "IOTypes/IOTypeFactory.h"
#include <functional>

class IO {
private:
	std::unique_ptr<IOInterface> m_IO;
public:
	IO(IOType type);

	std::function<void(std::string name, std::vector<int> keyIds, std::function<void()> action, KeyDetectionAction detectionAction, InputType inputType, bool overrideGuiFocused)> addKeyBind() {
		return [this](std::string name, std::vector<int> keyIds, std::function<void()> action, KeyDetectionAction detectionAction, InputType inputType, bool overrideGuiFocused) {
			m_IO->addKeyToCheckFor(name, keyIds, action, detectionAction, inputType, overrideGuiFocused);
		};
	}

	std::function<void(void* windowPointer, float currentFrame, bool isGUIActive)> updateIO() {
		return [this](void* windowPointer, float currentFrame, bool isGUIActive) {
			m_IO->processDisplayUpdate(windowPointer, currentFrame, isGUIActive);
		};
	}

	std::function<void(std::string name, std::vector<int> keyids)> updateKeyToCheckFor() {
		return [this](std::string name, std::vector<int> keyids) {
			m_IO->updateKeyToCheckFor(name, keyids);
		};
	}

	void setCameraUpdateCallBack(std::function<void(int,int,double,double)> tmpCallback) {
		m_IO->setUpdateCameraCallback(tmpCallback);
	}

	IOInterface* getInterface() {
		return std::move(m_IO.get());
	}

};