#include <Windows.h>
#include <iostream>
#include "EmulatorCore.h"
#include "VulkanRenderer.h"
#include "Config.h"
#include "InputManager.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow) {
    Config& config = Config::GetInstance();
    config.Load("config/settings.json");

    EmulatorCore& core = EmulatorCore::GetInstance();
    if (!core.Initialize()) {
        MessageBoxA(NULL, "Failed to initialize Eden Emulator", "Error", MB_ICONERROR);
        return 1;
    }

    VulkanRenderer& renderer = VulkanRenderer::GetInstance();
    if (!renderer.Initialize(hInstance)) {
        MessageBoxA(NULL, "Failed to initialize Vulkan renderer", "Error", MB_ICONERROR);
        return 1;
    }

    InputManager::GetInstance().Initialize();

    core.Run();
    renderer.Shutdown();
    return 0;
}
