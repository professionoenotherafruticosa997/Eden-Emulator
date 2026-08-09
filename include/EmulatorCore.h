#pragma once
#include <cstdint>
#include <string>
#include "MemoryManager.h"

class CPUInterpreter;
class GPUManager;
class SaveState;

enum class EmulationState { Stopped, Running, Paused };

class EmulatorCore {
private:
    EmulationState m_state = EmulationState::Stopped;
    std::string m_romPath;
    float m_speedMultiplier = 1.0f;
    int m_frameLimit = 60;
    MemoryManager* m_memory = nullptr;
    CPUInterpreter* m_cpu = nullptr;
    GPUManager* m_gpu = nullptr;
    SaveState* m_saveState = nullptr;
    EmulatorCore() = default;
    void FrameLimit(std::chrono::high_resolution_clock::time_point frameStart);
public:
    static EmulatorCore& GetInstance();
    bool Initialize();
    bool LoadROM(const std::string& path);
    void Run();
    void Pause(); void Resume(); void Stop();
    void SetSpeedMultiplier(float mult);
    EmulationState GetState() const { return m_state; }
};
