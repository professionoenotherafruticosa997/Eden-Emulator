#include "GPUManager.h"
#include "VulkanRenderer.h"

GPUManager& GPUManager::GetInstance() { static GPUManager i; return i; }

GPUManager* GPUManager::Allocate() {
    m_resolutionScale = 1.0f;
    m_vsyncEnabled = true;
    m_frameLimit = 60;
    m_shaderCache.clear();
    return this;
}

void GPUManager::RenderFrame() {
    VulkanRenderer::GetInstance().RenderFrame();
}

void GPUManager::SetResolutionScale(float scale) {
    m_resolutionScale = scale;
    VulkanRenderer::GetInstance().SetResolutionScale(scale);
}

void GPUManager::SetVSync(bool enabled) {
    m_vsyncEnabled = enabled;
}

void GPUManager::SetROMInfo(const ROMInfo& info) {
    m_romInfo = info;
}

void GPUManager::CacheShader(const std::string& hash, const std::vector<uint32_t>& spirv) {
    m_shaderCache[hash] = spirv;
}

bool GPUManager::HasCachedShader(const std::string& hash) const {
    return m_shaderCache.find(hash) != m_shaderCache.end();
}

const std::vector<uint32_t>& GPUManager::GetCachedShader(const std::string& hash) const {
    static const std::vector<uint32_t> empty;
    auto it = m_shaderCache.find(hash);
    return it != m_shaderCache.end() ? it->second : empty;
}
