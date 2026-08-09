#include "EmulatorCore.h"
#include "CPUInterpreter.h"
#include "GPUManager.h"
#include "MemoryManager.h"
#include "SaveState.h"
#include <chrono>

EmulatorCore& EmulatorCore::GetInstance() { static EmulatorCore i; return i; }

bool EmulatorCore::Initialize() {
    m_memory = MemoryManager::GetInstance().Allocate();
    m_cpu = CPUInterpreter::GetInstance().Allocate();
    m_gpu = GPUManager::GetInstance().Allocate();
    m_saveState = SaveState::GetInstance().Allocate();

    m_state = EmulationState::Stopped;
    m_speedMultiplier = 1.0f;
    m_frameLimit = 60;
    return true;
}

bool EmulatorCore::LoadROM(const std::string& path) {
    m_romPath = path;
    if (!m_memory->LoadROM(path)) return false;
    if (!m_cpu->Reset()) return false;
    m_gpu->SetROMInfo(m_memory->GetROMInfo());
    m_state = EmulationState::Paused;
    return true;
}

void EmulatorCore::Run() {
    m_state = EmulationState::Running;
    auto lastTime = std::chrono::high_resolution_clock::now();

    while (m_state != EmulationState::Stopped) {
        auto now = std::chrono::high_resolution_clock::now();
        float delta = std::chrono::duration<float>(now - lastTime).count();
        lastTime = now;

        if (m_state == EmulationState::Running) {
            m_cpu->ExecuteFrame(delta * m_speedMultiplier);
            m_gpu->RenderFrame();
        }

        FrameLimit(lastTime);
    }
}

void EmulatorCore::FrameLimit(std::chrono::high_resolution_clock::time_point frameStart) {
    if (m_frameLimit <= 0) return;
    auto frameEnd = std::chrono::high_resolution_clock::now();
    float frameTime = std::chrono::duration<float>(frameEnd - frameStart).count();
    float targetTime = 1.0f / m_frameLimit;
    if (frameTime < targetTime) {
        Sleep((DWORD)((targetTime - frameTime) * 1000.0f));
    }
}

void EmulatorCore::Pause() { m_state = EmulationState::Paused; }
void EmulatorCore::Resume() { if (m_state == EmulationState::Paused) m_state = EmulationState::Running; }
void EmulatorCore::Stop() { m_state = EmulationState::Stopped; }
void EmulatorCore::SetSpeedMultiplier(float mult) { m_speedMultiplier = mult; }
