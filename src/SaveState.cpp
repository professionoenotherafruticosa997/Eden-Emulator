#include "SaveState.h"
#include "MemoryManager.h"
#include "CPUInterpreter.h"
#include <fstream>

SaveState& SaveState::GetInstance() { static SaveState i; return i; }

SaveState* SaveState::Allocate() { return this; }

bool SaveState::Save(const std::string& path, int slot) {
    std::string filename = path + "/save_" + std::to_string(slot) + ".state";
    std::ofstream file(filename, std::ios::binary);
    if (!file.is_open()) return false;

    uint32_t version = 1;
    file.write((char*)&version, sizeof(version));

    auto& regs = CPUInterpreter::GetInstance().GetRegisters();
    file.write((char*)&regs, sizeof(regs));

    auto& mem = MemoryManager::GetInstance();
    uint64_t ramSize = 4 * 1024 * 1024 * 1024ULL;
    file.write((char*)&ramSize, sizeof(ramSize));

    return true;
}

bool SaveState::Load(const std::string& path, int slot) {
    std::string filename = path + "/save_" + std::to_string(slot) + ".state";
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) return false;

    uint32_t version;
    file.read((char*)&version, sizeof(version));
    if (version != 1) return false;

    CPURegisters regs;
    file.read((char*)&regs, sizeof(regs));

    return true;
}

std::vector<int> SaveState::GetSaveSlots(const std::string& path) {
    std::vector<int> slots;
    for (int i = 0; i < 10; i++) {
        std::string filename = path + "/save_" + std::to_string(i) + ".state";
        if (std::filesystem::exists(filename)) slots.push_back(i);
    }
    return slots;
}
