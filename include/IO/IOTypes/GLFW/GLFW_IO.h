#pragma once
#include <GLFW/glfw3.h>
#include <iostream>
#include "./../IOTypeInterface.h"


struct KeyProcessor {
    std::map<int, bool> wasKeyPressed;
    std::vector<int> inputIds;  // Can be key IDs or mouse button IDs
    std::function<void()> action;
    KeyDetectionAction detectionAction;  // Detection action (OnPress or OnHold)
    InputType inputType;  // Type of input (keyboard or mouse button)
    bool overrideImGuiFocused;

    bool isTriggered = false;  // Tracks if the action has already been triggered

    KeyProcessor() {}

    KeyProcessor(std::vector<int> tmpInputIds, std::function<void()> action, KeyDetectionAction detectionAction, InputType inputType, bool overrideImGuiFocused)
        : inputIds(std::move(tmpInputIds)), action(action), detectionAction(detectionAction), inputType(inputType), overrideImGuiFocused(overrideImGuiFocused) {
        for (int id : inputIds) {
            wasKeyPressed[id] = false;
        }
    }

    bool process(GLFWwindow* window) {
        bool allInputsPressed = true;

        // Check the current state of all inputs (keys or mouse buttons)
        for (int id : inputIds) {
            int inputState = (inputType == KeyboardInput)
                ? glfwGetKey(window, id)
                : glfwGetMouseButton(window, id);

            if (inputState != GLFW_PRESS) {
                allInputsPressed = false;
                break;
            }
        }

        if (detectionAction == OnPress) {
            // OnPress logic: Trigger the action once when all inputs are pressed
            if (allInputsPressed && !isTriggered) {
                action();  // Trigger the action once
                isTriggered = true;  // Mark the action as triggered
            }
            else if (!allInputsPressed) {
                isTriggered = false;  // Reset the trigger when not all inputs are pressed
            }
        }
        else if (detectionAction == OnHold) {
            // OnHold logic: Trigger the action continuously while all inputs are pressed
            if (allInputsPressed) {
                action();  // Trigger the action continuously while holding
                isTriggered = true;
            }
            else {
                isTriggered = false;  // Stop triggering if any input is released
            }
        }

        return isTriggered;
    }
};


class GLFW_IO : public IOInterface{
private:

    std::map<std::string, KeyProcessor> keyProcessors;
    std::map<std::string, std::function<void(double,double)>> mouseScrollWheelCallbacks;
    std::map<std::string, std::string> nameToKeyStringRepresentation;
    float deltaTime;
    float lastFrameTime;
    bool isGuiActive = false;

    std::vector<int> IOtoGLFWVectorMapping(std::vector<int> IOIds);
    int IOtoGLFW_KeyMapping(int key);
    std::string getStringRepresentation(std::vector<int> IOKeyIds);

   

public:
	GLFW_IO();
    void addKeyToCheckFor(std::string name, std::vector<int> keyIds, std::function<void()> action, KeyDetectionAction detectionAction, InputType inputType, bool overrideGuiFocused) override;
    void processDisplayUpdate(void* windowPointer, float currentFrame, bool isGUIActive) override;
    void updateKeyToCheckFor(std::string name, std::vector<int> keyIds) override;
    void addMouseScrollWheelCheck(std::string name, std::function<void(double, double)> action) override;
    void setCursorPosition(double x, double y) override;
    void setInputMode(InputModeActions action) override;
    std::map<std::string, std::string>& getBoundKeys() override;
    void updateScrollWheelInfo(double xOffset, double yOffset);
};