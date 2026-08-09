#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include <unordered_map>

struct ROMInfo {
    std::string title;
    std::string gameId;
    uint64_t size = 0;
};

class MemoryManager {
private:
    static constexpr uint64_t ROM_BASE = 0x80000000;
    std::vector<uint8_t> m_ram;
    std::vector<uint8_t> m_rom;
    std::unordered_map<std::string, std::string> m_keys;
    std::vector<std::string> m_firmware;
    ROMInfo m_romInfo;
    MemoryManager() = default;
    void ParseROMHeader();
public:
    static MemoryManager& GetInstance();
    MemoryManager* Allocate();
    bool LoadROM(const std::string& path);
    bool LoadKeys(const std::string& path);
    bool LoadFirmware(const std::string& path);
    uint8_t Read8(uint64_t addr);
    uint16_t Read16(uint64_t addr);
    uint32_t Read32(uint64_t addr);
    uint64_t Read64(uint64_t addr);
    void Write8(uint64_t addr, uint8_t val);
    void Write16(uint64_t addr, uint16_t val);
    void Write32(uint64_t addr, uint32_t val);
    void Write64(uint64_t addr, uint64_t val);
    const ROMInfo& GetROMInfo() const { return m_romInfo; }
};
