#pragma once

#include <vulkan/vulkan.h>

class VulkanQueue
{
public:
	VulkanQueue();
	~VulkanQueue();

public:
	void init(VkDevice device, VkSwapchainKHR swapchain, uint32_t queueFamily, uint32_t queueIndex);
	void destroy();
	uint32_t acquireNextImage();
	void submitSync(VkCommandBuffer commandBuffer);
	void submitAsync(VkCommandBuffer commandBuffer);
	void present(uint32_t imageIndex);
	void waitIdle();

private:
	void createSemaphores();

private:
	VkDevice mDevice = VK_NULL_HANDLE;
	VkSwapchainKHR mSwapChain = VK_NULL_HANDLE;
	VkQueue mQueue = VK_NULL_HANDLE;
	VkSemaphore mRenderCompleteSemaphore = VK_NULL_HANDLE;
	VkSemaphore mPresentCompleteSemaphore = VK_NULL_HANDLE;
};