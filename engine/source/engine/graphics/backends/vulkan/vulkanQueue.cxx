#include "engine/graphics/backends/vulkan/vulkanQueue.hxx"
#include "engine/graphics/backends/vulkan/vulkanUtils.hxx"

#include <cstdio>
#include <stdexcept>

VulkanQueue::VulkanQueue()
{
}

VulkanQueue::~VulkanQueue()
{
}

void VulkanQueue::init(VkDevice device, VkSwapchainKHR swapchain, uint32_t queueFamily, uint32_t queueIndex)
{
	mDevice = device;
	mSwapChain = swapchain;

	vkGetDeviceQueue(device, queueFamily, queueIndex, &mQueue);
	printf("VulkanQueue: Queue acquired\n");

	createSemaphores();
}

void VulkanQueue::destroy()
{
	vkDestroySemaphore(mDevice, mPresentCompleteSemaphore, nullptr);
	vkDestroySemaphore(mDevice, mRenderCompleteSemaphore, nullptr);
}

uint32_t VulkanQueue::acquireNextImage()
{
	uint32_t imageIndex = 0;
	VkResult result = vkAcquireNextImageKHR(mDevice, mSwapChain, UINT64_MAX, mPresentCompleteSemaphore, nullptr, &imageIndex);
	if (result != VK_SUCCESS)
	{
		throw std::runtime_error("VulkanQueue: Acquire next image problem\n");
	}
	return imageIndex;
}

void VulkanQueue::submitSync(VkCommandBuffer commandBuffer)
{
	VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.pNext = nullptr;
	submitInfo.waitSemaphoreCount = 0;
	submitInfo.pWaitSemaphores = VK_NULL_HANDLE;
	submitInfo.pWaitDstStageMask = VK_NULL_HANDLE;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffer;
	submitInfo.signalSemaphoreCount = 0;
	submitInfo.pSignalSemaphores = VK_NULL_HANDLE;

	VkResult res = vkQueueSubmit(mQueue, 1, &submitInfo, nullptr);
	if (res != VK_SUCCESS)
	{
		throw std::runtime_error("VulkanQueue: SubmitSync problem\n");
	}
}

void VulkanQueue::submitAsync(VkCommandBuffer commandBuffer)
{
	VkPipelineStageFlags waitFlags = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;

	VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.pNext = nullptr;
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = &mPresentCompleteSemaphore;
	submitInfo.pWaitDstStageMask = &waitFlags;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffer;
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = &mRenderCompleteSemaphore;

	VkResult result = vkQueueSubmit(mQueue, 1, &submitInfo, nullptr);
	if (result != VK_SUCCESS)
	{
		throw std::runtime_error("VulkanQueue: SubmitAsync problem\n");
	}
}

void VulkanQueue::present(uint32_t imageIndex)
{
	VkPresentInfoKHR presentInfo{};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	presentInfo.pNext = nullptr;
	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = &mRenderCompleteSemaphore;
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = &mSwapChain;
	presentInfo.pImageIndices = &imageIndex;

	VkResult result = vkQueuePresentKHR(mQueue, &presentInfo);
	if (result != VK_SUCCESS)
	{
		throw std::runtime_error("VulkanQueue: Present problem\n");
	}

	waitIdle();
}

void VulkanQueue::waitIdle()
{
	vkQueueWaitIdle(mQueue);
}

void VulkanQueue::createSemaphores()
{
	mPresentCompleteSemaphore = createSemaphore(mDevice);
	mRenderCompleteSemaphore = createSemaphore(mDevice);

	printf("VulkanQueue: Semaphores created\n");
}
