#include "MemoryManager.h"
#include <fstream>
#include <filesystem>

namespace fs = std::filesystem;

MemoryManager& MemoryManager::GetInstance() { static MemoryManager i; return i; }

MemoryManager* MemoryManager::Allocate() {
    m_ram.resize(4 * 1024 * 1024 * 1024ULL); // 4GB RAM
    m_rom.clear();
    m_keys.clear();
    m_firmware.clear();
    return this;
}

bool MemoryManager::LoadROM(const std::string& path) {
    if (!fs::exists(path)) return false;

    std::ifstream file(path, std::ios::binary);
    if (!file.is_open()) return false;

    file.seekg(0, std::ios::end);
    size_t size = file.tellg();
    file.seekg(0, std::ios::beg);

    m_rom.resize(size);
    file.read(reinterpret_cast<char*>(m_rom.data()), size);

    ParseROMHeader();
    return true;
}

void MemoryManager::ParseROMHeader() {
    if (m_rom.size() < 0x100) return;

    m_romInfo.title = std::string(reinterpret_cast<char*>(m_rom.data() + 0x200), 0x200);
    m_romInfo.title.erase(m_romInfo.title.find('\0'));
    m_romInfo.gameId = std::string(reinterpret_cast<char*>(m_rom.data() + 0x40), 16);
    m_romInfo.gameId.erase(m_romInfo.gameId.find('\0'));
    m_romInfo.size = m_rom.size();
}

bool MemoryManager::LoadKeys(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) return false;

    std::string line;
    while (std::getline(file, line)) {
        size_t eq = line.find('=');
        if (eq != std::string::npos) {
            std::string key = line.substr(0, eq);
            std::string val = line.substr(eq + 1);
            m_keys[key] = val;
        }
    }
    return true;
}

bool MemoryManager::LoadFirmware(const std::string& path) {
    if (!fs::exists(path)) return false;

    for (const auto& entry : fs::directory_iterator(path)) {
        if (entry.path().extension() == ".nca") {
            m_firmware.push_back(entry.path().string());
        }
    }
    return !m_firmware.empty();
}

uint8_t MemoryManager::Read8(uint64_t addr) {
    if (addr < m_ram.size()) return m_ram[addr];
    if (addr >= ROM_BASE && addr < ROM_BASE + m_rom.size()) return m_rom[addr - ROM_BASE];
    return 0;
}

uint16_t MemoryManager::Read16(uint64_t addr) {
    return Read8(addr) | (Read8(addr + 1) << 8);
}

uint32_t MemoryManager::Read32(uint64_t addr) {
    return Read8(addr) | (Read8(addr + 1) << 8) | (Read8(addr + 2) << 16) | (Read8(addr + 3) << 24);
}

uint64_t MemoryManager::Read64(uint64_t addr) {
    return (uint64_t)Read32(addr) | ((uint64_t)Read32(addr + 4) << 32);
}

void MemoryManager::Write8(uint64_t addr, uint8_t val) {
    if (addr < m_ram.size()) m_ram[addr] = val;
}

void MemoryManager::Write16(uint64_t addr, uint16_t val) {
    Write8(addr, val & 0xFF); Write8(addr + 1, val >> 8);
}

void MemoryManager::Write32(uint64_t addr, uint32_t val) {
    Write16(addr, val & 0xFFFF); Write16(addr + 2, val >> 16);
}

void MemoryManager::Write64(uint64_t addr, uint64_t val) {
    Write32(addr, (uint32_t)(val & 0xFFFFFFFF)); Write32(addr + 4, (uint32_t)(val >> 32));
}
