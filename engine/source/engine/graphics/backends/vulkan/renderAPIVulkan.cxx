#include "engine/graphics/backends/vulkan/renderAPIVulkan.hxx"

//#include "engine/window/backends/glfw/glfwWindow.hxx"
#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"

#include <cstdio>
#include <stdexcept>
#include <vector>

RenderAPIVulkan::RenderAPIVulkan()
{
	printf("RenderAPIVulkan: initialization\n");

	VkApplicationInfo appInfo{};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = "Render Engine";
	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.pEngineName = "Ligh";
	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.apiVersion = VK_API_VERSION_1_3;

	VkInstanceCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo = &appInfo;

	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions;

	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	createInfo.enabledExtensionCount = glfwExtensionCount;
	createInfo.ppEnabledExtensionNames = glfwExtensions;
	createInfo.enabledLayerCount = 0;

	VkResult result = vkCreateInstance(&createInfo, nullptr, &mInstance);
	if (result != VK_SUCCESS)
	{
		throw std::runtime_error("RenderAPIVulkan: Create Instance Problem\n");
	}

	uint32_t extensionCount = 0;
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

	std::vector<VkExtensionProperties> extensions(extensionCount);
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

	printf("RenderAPIVulkan: Available extensions:\n");
	for (const auto& extension : extensions) {
		printf("\t%s\n", extension.extensionName);
	}
}

RenderAPIVulkan::~RenderAPIVulkan()
{
	vkDestroyInstance(mInstance, nullptr);
}

void RenderAPIVulkan::setWindow(Window* window)
{
	//GLFWWindow* glfwWindow = dynamic_cast<GLFWWindow*>(window);
}
