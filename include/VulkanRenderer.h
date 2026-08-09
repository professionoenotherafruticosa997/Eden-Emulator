#pragma once
#include <vulkan/vulkan.h>
#include <Windows.h>
#include <vector>

class VulkanRenderer {
private:
    HINSTANCE m_hInstance = nullptr;
    VkInstance m_instance = VK_NULL_HANDLE;
    VkPhysicalDevice m_physicalDevice = VK_NULL_HANDLE;
    VkDevice m_device = VK_NULL_HANDLE;
    VkSwapchainKHR m_swapChain = VK_NULL_HANDLE;
    VkRenderPass m_renderPass = VK_NULL_HANDLE;
    VkPipeline m_pipeline = VK_NULL_HANDLE;
    VulkanRenderer() = default;
    bool CreateInstance();
    bool SelectPhysicalDevice();
    bool CreateLogicalDevice();
    bool CreateSwapChain();
    bool CreateRenderPass();
    bool CreatePipeline();
    bool CreateFramebuffers();
    bool CreateCommandBuffers();
public:
    static VulkanRenderer& GetInstance();
    bool Initialize(HINSTANCE hInstance);
    void RenderFrame();
    void SetResolutionScale(float scale);
    void Shutdown();
};
