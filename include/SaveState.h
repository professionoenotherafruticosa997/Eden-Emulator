#pragma once
#include <string>
#include <vector>
#include <filesystem>

class SaveState {
private:
    SaveState() = default;
public:
    static SaveState& GetInstance();
    SaveState* Allocate();
    bool Save(const std::string& path, int slot);
    bool Load(const std::string& path, int slot);
    std::vector<int> GetSaveSlots(const std::string& path);
};
