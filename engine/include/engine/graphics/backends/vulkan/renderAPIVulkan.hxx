#pragma once
#include <cstdint>
#include <vector>
#include <vulkan/vulkan.h>

#include "engine/graphics/renderAPI.hxx"
#include "engine/graphics/backends/vulkan/vulkanPhysicalDevice.hxx"

class RenderAPIVulkan : public RenderAPI
{
public:
	RenderAPIVulkan();
	~RenderAPIVulkan();

	virtual void setWindow(Window* window);
    
private:
    void createInstance();
    void createDebugCallback();

private:
	VkInstance mInstance = VK_NULL_HANDLE;
    VkDebugUtilsMessengerEXT mDebugMessenger = VK_NULL_HANDLE;
    VkSurfaceKHR mWindowSurface;
};
