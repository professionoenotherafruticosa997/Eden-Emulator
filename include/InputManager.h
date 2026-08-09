#pragma once
#include <string>
#include <unordered_map>
#include <Windows.h>
#include <Xinput.h>
#pragma comment(lib, "Xinput.lib")

class InputManager {
private:
    bool m_controllerConnected = false;
    float m_stickDeadzone = 0.15f;
    XINPUT_STATE m_state{};
    std::unordered_map<std::string, int> m_buttonMapping;
    InputManager() = default;
public:
    static InputManager& GetInstance();
    void Initialize();
    void Update();
    bool IsButtonPressed(const std::string& button);
    bool IsControllerConnected() const;
    float GetStickDeadzone() const { return m_stickDeadzone; }
};
