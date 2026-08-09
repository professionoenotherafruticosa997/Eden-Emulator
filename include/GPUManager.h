#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include "MemoryManager.h"

class GPUManager {
private:
    float m_resolutionScale = 1.0f;
    bool m_vsyncEnabled = true;
    int m_frameLimit = 60;
    ROMInfo m_romInfo;
    std::unordered_map<std::string, std::vector<uint32_t>> m_shaderCache;
    GPUManager() = default;
public:
    static GPUManager& GetInstance();
    GPUManager* Allocate();
    void RenderFrame();
    void SetResolutionScale(float scale);
    void SetVSync(bool enabled);
    void SetROMInfo(const ROMInfo& info);
    void CacheShader(const std::string& hash, const std::vector<uint32_t>& spirv);
    bool HasCachedShader(const std::string& hash) const;
    const std::vector<uint32_t>& GetCachedShader(const std::string& hash) const;
    float GetResolutionScale() const { return m_resolutionScale; }
};
