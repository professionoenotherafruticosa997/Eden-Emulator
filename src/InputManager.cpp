#include "InputManager.h"
#include "Config.h"

InputManager& InputManager::GetInstance() { static InputManager i; return i; }

void InputManager::Initialize() {
    m_controllerConnected = false;
    m_stickDeadzone = 0.15f;
    m_buttonMapping = {
        {"A", VK_GAMEPAD_A}, {"B", VK_GAMEPAD_B},
        {"X", VK_GAMEPAD_X}, {"Y", VK_GAMEPAD_Y},
        {"L", VK_GAMEPAD_LEFT_SHOULDER}, {"R", VK_GAMEPAD_RIGHT_SHOULDER},
        {"ZL", VK_GAMEPAD_LEFT_TRIGGER}, {"ZR", VK_GAMEPAD_RIGHT_TRIGGER},
        {"Plus", VK_GAMEPAD_START}, {"Minus", VK_GAMEPAD_BACK},
        {"LS", VK_GAMEPAD_LEFT_THUMB}, {"RS", VK_GAMEPAD_RIGHT_THUMB},
        {"DPadUp", VK_GAMEPAD_DPAD_UP}, {"DPadDown", VK_GAMEPAD_DPAD_DOWN},
        {"DPadLeft", VK_GAMEPAD_DPAD_LEFT}, {"DPadRight", VK_GAMEPAD_DPAD_RIGHT},
    };
}

void InputManager::Update() {
    if (m_controllerConnected) {
        XINPUT_STATE state;
        if (XInputGetState(0, &state) == ERROR_SUCCESS) {
            m_state = state;
        }
    }
}

bool InputManager::IsButtonPressed(const std::string& button) {
    auto it = m_buttonMapping.find(button);
    if (it == m_buttonMapping.end()) return false;
    return (GetAsyncKeyState(it->second) & 0x8000) != 0;
}

bool InputManager::IsControllerConnected() const { return m_controllerConnected; }
