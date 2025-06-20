#include "IO/IOTypes/GLFW/GLFW_IO.h"


static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    GLFW_IO* ioInstance = static_cast<GLFW_IO*>(glfwGetWindowUserPointer(window));
    if (ioInstance) {
        ioInstance->updateScrollWheelInfo(xoffset,yoffset);
    }

}

void GLFW_IO::updateScrollWheelInfo(double xOffset, double yOffset) {
    std::cout << "Detected Mouse Scroll xOffset: " << xOffset << " , yOffset: " << yOffset << std::endl;

    if (this->isGuiActive) { return; }

    for (const auto& [key, func] : mouseScrollWheelCallbacks) {
        if (func) {  // Check if function is valid
            func(xOffset,yOffset);
        }
    }

}

GLFW_IO::GLFW_IO() {

}

void GLFW_IO::addKeyToCheckFor(std::string name, std::vector<int> keyIds, std::function<void()> action, KeyDetectionAction detectionAction, InputType inputType, bool overrideGuiFocused) {
    nameToKeyStringRepresentation[name] = getStringRepresentation(keyIds);
    keyProcessors[name] = (KeyProcessor(std::move(IOtoGLFWVectorMapping(keyIds)), action, detectionAction, inputType, overrideGuiFocused));
}

void GLFW_IO::addMouseScrollWheelCheck(std::string name, std::function<void(double, double)> action) {
    mouseScrollWheelCallbacks[name] = action;
}

void GLFW_IO::processDisplayUpdate(void* windowPointer, float currentFrame, bool isGUIActive) {
    this->isGuiActive = isGUIActive;
    if (!isWindowPointerSet) {
        m_WindowPointer = windowPointer;
        isWindowPointerSet = true;
        glfwSetWindowUserPointer(static_cast<GLFWwindow*>(windowPointer), this);
        glfwSetScrollCallback(static_cast<GLFWwindow*>(windowPointer), scroll_callback);
    }

    GLFWwindow* window = static_cast<GLFWwindow*>(windowPointer);

    deltaTime = currentFrame - lastFrameTime;
    lastFrameTime = currentFrame;

    for (auto it = keyProcessors.begin(); it != keyProcessors.end(); ++it) {

        //Check to see if the individual key detection can happen when focused on ImGui windows
        //If it cannot happen when focused on an ImGui window, and if the user is currently focused on an ImGui window skip processing
        if (!it->second.overrideImGuiFocused  && this->isGuiActive) { continue; }

        it->second.process(window);
    }

    //If there is an active camera set. Need to sample the current glfw window information and send it to the camera
    if (isCameraCallBackSet) {
        int tempWindowWidth , tempWindowHeight;
        double tempCursPosX, tempCursPosY;

        glfwGetWindowSize(window, &tempWindowWidth, &tempWindowHeight);
        glfwGetCursorPos(window, &tempCursPosX, &tempCursPosY);

        updateCameraCallback(tempWindowWidth, tempWindowHeight, tempCursPosX, tempCursPosY);
    }

}
void GLFW_IO::updateKeyToCheckFor(std::string name, std::vector<int> keyIds) {
    nameToKeyStringRepresentation[name] = getStringRepresentation(keyIds);
    keyProcessors[name].inputIds = IOtoGLFWVectorMapping(keyIds);
}

std::map<std::string, std::string>& GLFW_IO::getBoundKeys() {
    return nameToKeyStringRepresentation;
}


//PRIVATE FUNCTIONS
std::vector<int> GLFW_IO::IOtoGLFWVectorMapping(std::vector<int> IOIds) {
    std::vector<int> returnIds;
    for (int IOid : IOIds) {
        returnIds.push_back(IOtoGLFW_KeyMapping(IOid));
    }
    return returnIds;
}

std::string GLFW_IO::getStringRepresentation(std::vector<int> IOKeyIds) {
    std::string tempString;

    std::vector<std::string> modifiers;
    std::vector<std::string> normalKeys;

    for (int key : IOKeyIds) {
        std::string keyName = getKeyName(key);
        if (keyName == "LEFT_SHIFT" || keyName == "LEFT_CONTROL" || keyName == "LEFT_ALT") {
            modifiers.push_back(keyName);
        }
        else {
            normalKeys.push_back(keyName);
        }
    }

    std::sort(modifiers.begin(), modifiers.end(), [](const std::string& a, const std::string& b) {
        return a < b;
        });

    for (const std::string& mod : modifiers) {
        if (!tempString.empty()) { tempString += " + "; }
        tempString += mod;
    }
    for (const std::string& key : normalKeys) {
        if (!tempString.empty()) { tempString += " + "; }
        tempString += key;
    }

    return tempString;
}


void GLFW_IO::setCursorPosition(double x, double y) {
    GLFWwindow* window = static_cast<GLFWwindow*>(m_WindowPointer);
    glfwSetCursorPos(window,x,y);
}

void GLFW_IO::setInputMode(InputModeActions action) {
    GLFWwindow* window = static_cast<GLFWwindow*>(m_WindowPointer);

    switch (action) {
    case CURSOR_DISABLE:
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        break;
    case CURSOR_NORMAL:
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        break;
    default:
        std::cerr << "Unknow Window Input Mode Action!!" << std::endl;
    }

}

int GLFW_IO::IOtoGLFW_KeyMapping(int key) {
    switch (key) {
    case IOKey::IO_SPACE: return GLFW_KEY_SPACE;
    case IOKey::IO_APOSTROPHE: return GLFW_KEY_APOSTROPHE;
    case IOKey::IO_COMMA: return GLFW_KEY_COMMA;
    case IOKey::IO_MINUS: return GLFW_KEY_MINUS;
    case IOKey::IO_PERIOD: return GLFW_KEY_PERIOD;
    case IOKey::IO_SLASH: return GLFW_KEY_SLASH;
    case IOKey::IO_0: return GLFW_KEY_0;
    case IOKey::IO_1: return GLFW_KEY_1;
    case IOKey::IO_2: return GLFW_KEY_2;
    case IOKey::IO_3: return GLFW_KEY_3;
    case IOKey::IO_4: return GLFW_KEY_4;
    case IOKey::IO_5: return GLFW_KEY_5;
    case IOKey::IO_6: return GLFW_KEY_6;
    case IOKey::IO_7: return GLFW_KEY_7;
    case IOKey::IO_8: return GLFW_KEY_8;
    case IOKey::IO_9: return GLFW_KEY_9;
    case IOKey::IO_SEMICOLON: return GLFW_KEY_SEMICOLON;
    case IOKey::IO_EQUAL: return GLFW_KEY_EQUAL;
    case IOKey::IO_A: return GLFW_KEY_A;
    case IOKey::IO_B: return GLFW_KEY_B;
    case IOKey::IO_C: return GLFW_KEY_C;
    case IOKey::IO_D: return GLFW_KEY_D;
    case IOKey::IO_E: return GLFW_KEY_E;
    case IOKey::IO_F: return GLFW_KEY_F;
    case IOKey::IO_G: return GLFW_KEY_G;
    case IOKey::IO_H: return GLFW_KEY_H;
    case IOKey::IO_I: return GLFW_KEY_I;
    case IOKey::IO_J: return GLFW_KEY_J;
    case IOKey::IO_K: return GLFW_KEY_K;
    case IOKey::IO_L: return GLFW_KEY_L;
    case IOKey::IO_M: return GLFW_KEY_M;
    case IOKey::IO_N: return GLFW_KEY_N;
    case IOKey::IO_O: return GLFW_KEY_O;
    case IOKey::IO_P: return GLFW_KEY_P;
    case IOKey::IO_Q: return GLFW_KEY_Q;
    case IOKey::IO_R: return GLFW_KEY_R;
    case IOKey::IO_S: return GLFW_KEY_S;
    case IOKey::IO_T: return GLFW_KEY_T;
    case IOKey::IO_U: return GLFW_KEY_U;
    case IOKey::IO_V: return GLFW_KEY_V;
    case IOKey::IO_W: return GLFW_KEY_W;
    case IOKey::IO_X: return GLFW_KEY_X;
    case IOKey::IO_Y: return GLFW_KEY_Y;
    case IOKey::IO_Z: return GLFW_KEY_Z;
    case IOKey::IO_LEFT_BRACKET: return GLFW_KEY_LEFT_BRACKET;
    case IOKey::IO_BACKSLASH: return GLFW_KEY_BACKSLASH;
    case IOKey::IO_RIGHT_BRACKET: return GLFW_KEY_RIGHT_BRACKET;
    case IOKey::IO_GRAVE_ACCENT: return GLFW_KEY_GRAVE_ACCENT;
    case IOKey::IO_WORLD_1: return GLFW_KEY_WORLD_1;
    case IOKey::IO_WORLD_2: return GLFW_KEY_WORLD_2;
    case IOKey::IO_ESCAPE: return GLFW_KEY_ESCAPE;
    case IOKey::IO_ENTER: return GLFW_KEY_ENTER;
    case IOKey::IO_TAB: return GLFW_KEY_TAB;
    case IOKey::IO_BACKSPACE: return GLFW_KEY_BACKSLASH;
    case IOKey::IO_INSTERT: return GLFW_KEY_INSERT;
    case IOKey::IO_DELETE: return GLFW_KEY_DELETE;
    case IOKey::IO_RIGHT: return GLFW_KEY_RIGHT;
    case IOKey::IO_LEFT: return GLFW_KEY_LEFT;
    case IOKey::IO_DOWN: return GLFW_KEY_DOWN;
    case IOKey::IO_UP: return GLFW_KEY_UP;
    case IOKey::IO_PAGE_DOWN: return GLFW_KEY_PAGE_DOWN;
    case IOKey::IO_PAGE_UP: return GLFW_KEY_PAGE_UP;
    case IOKey::IO_HOME: return GLFW_KEY_HOME;
    case IOKey::IO_END: return GLFW_KEY_END;
    case IOKey::IO_CAPS_LOCK: return GLFW_KEY_CAPS_LOCK;
    case IOKey::IO_SCROLL_LOCK: return GLFW_KEY_SCROLL_LOCK;
    case IOKey::IO_NUM_LOCK: return GLFW_KEY_NUM_LOCK;
    case IOKey::IO_PRINT_SCREEN: return GLFW_KEY_PRINT_SCREEN;
    case IOKey::IO_PAUSE: return GLFW_KEY_PAUSE;
    case IOKey::IO_F1: return GLFW_KEY_F1;
    case IOKey::IO_F2: return GLFW_KEY_F2;
    case IOKey::IO_F3: return GLFW_KEY_F3;
    case IOKey::IO_F4: return GLFW_KEY_F4;
    case IOKey::IO_F5: return GLFW_KEY_F5;
    case IOKey::IO_F6: return GLFW_KEY_F6;
    case IOKey::IO_F7: return GLFW_KEY_F7;
    case IOKey::IO_F8: return GLFW_KEY_F8;
    case IOKey::IO_F9: return GLFW_KEY_F9;
    case IOKey::IO_F10: return GLFW_KEY_F10;
    case IOKey::IO_F11: return GLFW_KEY_F11;
    case IOKey::IO_F12: return GLFW_KEY_F12;
    case IOKey::IO_F13: return GLFW_KEY_F13;
    case IOKey::IO_F14: return GLFW_KEY_F14;
    case IOKey::IO_F15: return GLFW_KEY_F15;
    case IOKey::IO_F16: return GLFW_KEY_F16;
    case IOKey::IO_F17: return GLFW_KEY_F17;
    case IOKey::IO_F18: return GLFW_KEY_F18;
    case IOKey::IO_F19: return GLFW_KEY_F19;
    case IOKey::IO_F20: return GLFW_KEY_F20;
    case IOKey::IO_F21: return GLFW_KEY_F21;
    case IOKey::IO_F22: return GLFW_KEY_F22;
    case IOKey::IO_F23: return GLFW_KEY_F23;
    case IOKey::IO_F24: return GLFW_KEY_F24;
    case IOKey::IO_F25: return GLFW_KEY_F25;
    case IOKey::IO_KP_0: return GLFW_KEY_KP_0;
    case IOKey::IO_KP_1: return GLFW_KEY_KP_1;
    case IOKey::IO_KP_2: return GLFW_KEY_KP_2;
    case IOKey::IO_KP_3: return GLFW_KEY_KP_3;
    case IOKey::IO_KP_4: return GLFW_KEY_KP_4;
    case IOKey::IO_KP_5: return GLFW_KEY_KP_5;
    case IOKey::IO_KP_6: return GLFW_KEY_KP_6;
    case IOKey::IO_KP_7: return GLFW_KEY_KP_7;
    case IOKey::IO_KP_8: return GLFW_KEY_KP_8;
    case IOKey::IO_KP_9: return GLFW_KEY_KP_9;
    case IOKey::IO_KP_DECIMAL: return GLFW_KEY_KP_DECIMAL;
    case IOKey::IO_KP_DIVIDE: return GLFW_KEY_KP_DIVIDE;
    case IOKey::IO_KP_MULTIPLY: return GLFW_KEY_KP_MULTIPLY;
    case IOKey::IO_KP_SUBTRACT: return GLFW_KEY_KP_SUBTRACT;
    case IOKey::IO_KP_ADD: return GLFW_KEY_KP_ADD;
    case IOKey::IO_KP_ENTER: return GLFW_KEY_KP_ENTER;
    case IOKey::IO_KP_EQUAL: return GLFW_KEY_KP_EQUAL;
    case IOKey::IO_LEFT_SHIFT: return GLFW_KEY_LEFT_SHIFT;
    case IOKey::IO_LEFT_CONTROL: return GLFW_KEY_LEFT_CONTROL;
    case IOKey::IO_LEFT_ALT: return GLFW_KEY_LEFT_ALT;
    case IOKey::IO_LEFT_SUPER: return GLFW_KEY_LEFT_SUPER;
    case IOKey::IO_RIGHT_SHIFT: return GLFW_KEY_RIGHT_SHIFT;
    case IOKey::IO_RIGHT_CONTROL: return GLFW_KEY_RIGHT_CONTROL;
    case IOKey::IO_RIGHT_ALT: return GLFW_KEY_RIGHT_ALT;
    case IOKey::IO_RIGHT_SUPER: return GLFW_KEY_RIGHT_SUPER;
    case IOKey::IO_MENU: return GLFW_KEY_MENU;
    case IOKey::IO_MOUSE_BUTTON_1: return GLFW_MOUSE_BUTTON_1;
    case IOKey::IO_MOUSE_BUTTON_2: return GLFW_MOUSE_BUTTON_2;
    }
}