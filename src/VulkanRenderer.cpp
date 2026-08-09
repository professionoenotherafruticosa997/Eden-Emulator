#include "VulkanRenderer.h"
#include <vulkan/vulkan.h>
#include <fstream>
#include <set>

VulkanRenderer& VulkanRenderer::GetInstance() { static VulkanRenderer i; return i; }

bool VulkanRenderer::Initialize(HINSTANCE hInstance) {
    m_hInstance = hInstance;

    if (!CreateInstance()) return false;
    if (!SelectPhysicalDevice()) return false;
    if (!CreateLogicalDevice()) return false;
    if (!CreateSwapChain()) return false;
    if (!CreateRenderPass()) return false;
    if (!CreatePipeline()) return false;
    if (!CreateFramebuffers()) return false;
    if (!CreateCommandBuffers()) return false;

    return true;
}

bool VulkanRenderer::CreateInstance() {
    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Eden Emulator";
    appInfo.applicationVersion = VK_MAKE_VERSION(0, 22, 0);
    appInfo.pEngineName = "Eden";
    appInfo.engineVersion = VK_MAKE_VERSION(0, 22, 0);
    appInfo.apiVersion = VK_API_VERSION_1_3;

    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;

    const char* extensions[] = {"VK_KHR_surface", "VK_KHR_win32_surface"};
    createInfo.enabledExtensionCount = 2;
    createInfo.ppEnabledExtensionNames = extensions;

    return vkCreateInstance(&createInfo, nullptr, &m_instance) == VK_SUCCESS;
}

bool VulkanRenderer::SelectPhysicalDevice() {
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(m_instance, &deviceCount, nullptr);
    if (deviceCount == 0) return false;

    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(m_instance, &deviceCount, devices.data());

    for (const auto& device : devices) {
        VkPhysicalDeviceProperties props;
        vkGetPhysicalDeviceProperties(device, &props);
        if (props.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
            m_physicalDevice = device;
            return true;
        }
    }

    m_physicalDevice = devices[0];
    return true;
}

bool VulkanRenderer::CreateLogicalDevice() {
    VkDeviceQueueCreateInfo queueInfo{};
    queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueInfo.queueFamilyIndex = 0;
    queueInfo.queueCount = 1;
    float priority = 1.0f;
    queueInfo.pQueuePriorities = &priority;

    const char* deviceExtensions[] = {"VK_KHR_swapchain"};

    VkDeviceCreateInfo deviceInfo{};
    deviceInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    deviceInfo.pQueueCreateInfos = &queueInfo;
    deviceInfo.queueCreateInfoCount = 1;
    deviceInfo.enabledExtensionCount = 1;
    deviceInfo.ppEnabledExtensionNames = deviceExtensions;

    return vkCreateDevice(m_physicalDevice, &deviceInfo, nullptr, &m_device) == VK_SUCCESS;
}

bool VulkanRenderer::CreateSwapChain() { return true; }
bool VulkanRenderer::CreateRenderPass() { return true; }
bool VulkanRenderer::CreatePipeline() { return true; }
bool VulkanRenderer::CreateFramebuffers() { return true; }
bool VulkanRenderer::CreateCommandBuffers() { return true; }

void VulkanRenderer::RenderFrame() {
    // Frame rendering logic
}

void VulkanRenderer::Shutdown() {
    if (m_device) vkDestroyDevice(m_device, nullptr);
    if (m_instance) vkDestroyInstance(m_instance, nullptr);
}
