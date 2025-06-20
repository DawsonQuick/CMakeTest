#pragma once
#include <string>
#include <map>
#include <algorithm>
#include <vector>
#include <functional>

enum KeyDetectionAction {
    OnPress,
    OnHold
};

enum InputType {
    KeyboardInput,
    MouseButtonInput
};

enum IOType {
    GLFW,
    MaxNumberOfIOTypes
};

enum InputModeActions {
    CURSOR_DISABLE,
    CURSOR_NORMAL
};

class IOInterface {
public:
	virtual ~IOInterface() = default;
    virtual void addKeyToCheckFor(std::string name, std::vector<int> keyIds, std::function<void()> action, KeyDetectionAction detectionAction, InputType inputType, bool overrideGuiFocused) = 0;
    virtual void addMouseScrollWheelCheck(std::string name, std::function<void(double, double)> action) = 0;
    virtual void processDisplayUpdate(void* windowPointer, float currentFrame, bool isGUIActive) = 0;
    virtual void updateKeyToCheckFor(std::string name, std::vector<int> keyIds) = 0;

    virtual void setCursorPosition(double x, double y) = 0;
    virtual void setInputMode(InputModeActions action) = 0;

    virtual std::map<std::string, std::string>& getBoundKeys() = 0;

    void setUpdateCameraCallback(std::function<void(int,int, double,double)> tmpCallback) {
        updateCameraCallback = std::move(tmpCallback);
        isCameraCallBackSet = true;
    }

protected:
    bool isCameraCallBackSet = false;
    bool isWindowPointerSet = false;
    std::function<void(int windowWidth, int windowHeight, double cursorX, double cursorY)> updateCameraCallback;
    void* m_WindowPointer;
};



enum IOKey {
    IO_SPACE,
    IO_APOSTROPHE,
    IO_COMMA,
    IO_MINUS,
    IO_PERIOD,
    IO_SLASH,
    IO_0,
    IO_1,
    IO_2,
    IO_3,
    IO_4,
    IO_5,
    IO_6,
    IO_7,
    IO_8,
    IO_9,
    IO_SEMICOLON,
    IO_EQUAL,
    IO_A,
    IO_B,
    IO_C,
    IO_D,
    IO_E,
    IO_F,
    IO_G,
    IO_H,
    IO_I,
    IO_J,
    IO_K,
    IO_L,
    IO_M,
    IO_N,
    IO_O,
    IO_P,
    IO_Q,
    IO_R,
    IO_S,
    IO_T,
    IO_U,
    IO_V,
    IO_W,
    IO_X,
    IO_Y,
    IO_Z,
    IO_LEFT_BRACKET,
    IO_BACKSLASH,
    IO_RIGHT_BRACKET,
    IO_GRAVE_ACCENT,
    IO_WORLD_1,
    IO_WORLD_2,
    IO_ESCAPE,
    IO_ENTER,
    IO_TAB,
    IO_BACKSPACE,
    IO_INSTERT,
    IO_DELETE,
    IO_RIGHT,
    IO_LEFT,
    IO_DOWN,
    IO_UP,
    IO_PAGE_DOWN,
    IO_PAGE_UP,
    IO_HOME,
    IO_END,
    IO_CAPS_LOCK,
    IO_SCROLL_LOCK,
    IO_NUM_LOCK,
    IO_PRINT_SCREEN,
    IO_PAUSE,
    IO_F1,
    IO_F2,
    IO_F3,
    IO_F4,
    IO_F5,
    IO_F6,
    IO_F7,
    IO_F8,
    IO_F9,
    IO_F10,
    IO_F11,
    IO_F12,
    IO_F13,
    IO_F14,
    IO_F15,
    IO_F16,
    IO_F17,
    IO_F18,
    IO_F19,
    IO_F20,
    IO_F21,
    IO_F22,
    IO_F23,
    IO_F24,
    IO_F25,
    IO_KP_0,
    IO_KP_1,
    IO_KP_2,
    IO_KP_3,
    IO_KP_4,
    IO_KP_5,
    IO_KP_6,
    IO_KP_7,
    IO_KP_8,
    IO_KP_9,
    IO_KP_DECIMAL,
    IO_KP_DIVIDE,
    IO_KP_MULTIPLY,
    IO_KP_SUBTRACT,
    IO_KP_ADD,
    IO_KP_ENTER,
    IO_KP_EQUAL,
    IO_LEFT_SHIFT,
    IO_LEFT_CONTROL,
    IO_LEFT_ALT,
    IO_LEFT_SUPER,
    IO_RIGHT_SHIFT,
    IO_RIGHT_CONTROL,
    IO_RIGHT_ALT,
    IO_RIGHT_SUPER,
    IO_MENU,
    IO_MOUSE_BUTTON_1,
    IO_MOUSE_BUTTON_2,

    IO_MAXNUMBEROFIOKEYENUMS

};

inline std::string getKeyName(int key) {
    switch (key) {
    case IOKey::IO_SPACE: return "SPACE";
    case IOKey::IO_APOSTROPHE: return "APOSTROPHE";
    case IOKey::IO_COMMA: return "COMMA";
    case IOKey::IO_MINUS: return "MINUS";
    case IOKey::IO_PERIOD: return "PERIOD";
    case IOKey::IO_SLASH: return "SLASH";
    case IOKey::IO_0: return "0";
    case IOKey::IO_1: return "1";
    case IOKey::IO_2: return "2";
    case IOKey::IO_3: return "3";
    case IOKey::IO_4: return "4";
    case IOKey::IO_5: return "5";
    case IOKey::IO_6: return "6";
    case IOKey::IO_7: return "7";
    case IOKey::IO_8: return "8";
    case IOKey::IO_9: return "9";
    case IOKey::IO_SEMICOLON: return "SEMICOLON";
    case IOKey::IO_EQUAL: return "EQUAL";
    case IOKey::IO_A: return "A";
    case IOKey::IO_B: return "B";
    case IOKey::IO_C: return "C";
    case IOKey::IO_D: return "D";
    case IOKey::IO_E: return "E";
    case IOKey::IO_F: return "F";
    case IOKey::IO_G: return "G";
    case IOKey::IO_H: return "H";
    case IOKey::IO_I: return "I";
    case IOKey::IO_J: return "J";
    case IOKey::IO_K: return "K";
    case IOKey::IO_L: return "L";
    case IOKey::IO_M: return "M";
    case IOKey::IO_N: return "N";
    case IOKey::IO_O: return "O";
    case IOKey::IO_P: return "P";
    case IOKey::IO_Q: return "Q";
    case IOKey::IO_R: return "R";
    case IOKey::IO_S: return "S";
    case IOKey::IO_T: return "T";
    case IOKey::IO_U: return "U";
    case IOKey::IO_V: return "V";
    case IOKey::IO_W: return "W";
    case IOKey::IO_X: return "X";
    case IOKey::IO_Y: return "Y";
    case IOKey::IO_Z: return "Z";
    case IOKey::IO_LEFT_BRACKET: return "LEFT_BRACKET";
    case IOKey::IO_BACKSLASH: return "BACKSLASH";
    case IOKey::IO_RIGHT_BRACKET: return "RIGHT_BRACKET";
    case IOKey::IO_GRAVE_ACCENT: return "GRAVE_ACCENT";
    case IOKey::IO_WORLD_1: return "WORLD_1";
    case IOKey::IO_WORLD_2: return "WORLD_2";
    case IOKey::IO_ESCAPE: return "ESCAPE";
    case IOKey::IO_ENTER: return "ENTER";
    case IOKey::IO_TAB: return "TAB";
    case IOKey::IO_BACKSPACE: return "BACKSLASH";
    case IOKey::IO_INSTERT: return "INSERT";
    case IOKey::IO_DELETE: return "DELETE";
    case IOKey::IO_RIGHT: return "RIGHT";
    case IOKey::IO_LEFT: return "LEFT";
    case IOKey::IO_DOWN: return "DOWN";
    case IOKey::IO_UP: return "UP";
    case IOKey::IO_PAGE_DOWN: return "PAGE_DOWN";
    case IOKey::IO_PAGE_UP: return "PAGE_UP";
    case IOKey::IO_HOME: return "HOME";
    case IOKey::IO_END: return "END";
    case IOKey::IO_CAPS_LOCK: return "CAPS_LOCK";
    case IOKey::IO_SCROLL_LOCK: return "SCROLL_LOCK";
    case IOKey::IO_NUM_LOCK: return "NUM_LOCK";
    case IOKey::IO_PRINT_SCREEN: return "PRINT_SCREEN";
    case IOKey::IO_PAUSE: return "PAUSE";
    case IOKey::IO_F1: return "F1";
    case IOKey::IO_F2: return "F2";
    case IOKey::IO_F3: return "F3";
    case IOKey::IO_F4: return "F4";
    case IOKey::IO_F5: return "F5";
    case IOKey::IO_F6: return "F6";
    case IOKey::IO_F7: return "F7";
    case IOKey::IO_F8: return "F8";
    case IOKey::IO_F9: return "F9";
    case IOKey::IO_F10: return "F10";
    case IOKey::IO_F11: return "F11";
    case IOKey::IO_F12: return "F12";
    case IOKey::IO_F13: return "F13";
    case IOKey::IO_F14: return "F14";
    case IOKey::IO_F15: return "F15";
    case IOKey::IO_F16: return "F16";
    case IOKey::IO_F17: return "F17";
    case IOKey::IO_F18: return "F18";
    case IOKey::IO_F19: return "F19";
    case IOKey::IO_F20: return "F20";
    case IOKey::IO_F21: return "F21";
    case IOKey::IO_F22: return "F22";
    case IOKey::IO_F23: return "F23";
    case IOKey::IO_F24: return "F24";
    case IOKey::IO_F25: return "F25";
    case IOKey::IO_KP_0: return "KP_0";
    case IOKey::IO_KP_1: return "KP_1";
    case IOKey::IO_KP_2: return "KP_2";
    case IOKey::IO_KP_3: return "KP_3";
    case IOKey::IO_KP_4: return "KP_4";
    case IOKey::IO_KP_5: return "KP_5";
    case IOKey::IO_KP_6: return "KP_6";
    case IOKey::IO_KP_7: return "KP_7";
    case IOKey::IO_KP_8: return "KP_8";
    case IOKey::IO_KP_9: return "KP_9";
    case IOKey::IO_KP_DECIMAL: return "KP_DECIMAL";
    case IOKey::IO_KP_DIVIDE: return "KP_DIVIDE";
    case IOKey::IO_KP_MULTIPLY: return "KP_MULTIPLY";
    case IOKey::IO_KP_SUBTRACT: return "KP_SUBTRACT";
    case IOKey::IO_KP_ADD: return "KP_ADD";
    case IOKey::IO_KP_ENTER: return "KP_ENTER";
    case IOKey::IO_KP_EQUAL: return "KP_EQUAL";
    case IOKey::IO_LEFT_SHIFT: return "LEFT_SHIFT";
    case IOKey::IO_LEFT_CONTROL: return "LEFT_CONTROL";
    case IOKey::IO_LEFT_ALT: return "LEFT_ALT";
    case IOKey::IO_LEFT_SUPER: return "LEFT_SUPER";
    case IOKey::IO_RIGHT_SHIFT: return "RIGHT_SHIFT";
    case IOKey::IO_RIGHT_CONTROL: return "RIGHT_CONTROL";
    case IOKey::IO_RIGHT_ALT: return "RIGHT_ALT";
    case IOKey::IO_RIGHT_SUPER: return "RIGHT_SUPER";
    case IOKey::IO_MENU: return "MENU"; 
    case IOKey::IO_MOUSE_BUTTON_1: return "MOUSE_BUTTON_1";
    case IOKey::IO_MOUSE_BUTTON_2: return "MOUSE_BUTTON_2";
    }
}