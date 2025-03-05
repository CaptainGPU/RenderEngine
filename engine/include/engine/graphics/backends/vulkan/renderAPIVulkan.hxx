#pragma once
#include <cstdint>
#include <vector>
#include <vulkan/vulkan.h>

#include "engine/graphics/renderAPI.hxx"
#include "engine/graphics/backends/vulkan/vulkanPhysicalDevice.hxx"

class RenderAPIVulkan : public RenderAPI
{
public:
	RenderAPIVulkan(Window* window);
	~RenderAPIVulkan();

private:
    void createInstance();
    void createDebugCallback();
    void setWindow(Window* window);
    void createDevice();

private:
	VkInstance mInstance = VK_NULL_HANDLE;
    VkDebugUtilsMessengerEXT mDebugMessenger = VK_NULL_HANDLE;
    VkSurfaceKHR mWindowSurface;
    VulkanPhysicalDevices mPhysicalDevices;
    uint32_t mQueueFamily = 0;
    VkDevice mDevice = VK_NULL_HANDLE;
};
