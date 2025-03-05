#include "engine/graphics/backends/vulkan/renderAPIVulkan.hxx"
#include "engine/graphics/backends/vulkan/vulkanUtils.hxx"
#include "engine/window/backends/glfw/glfwWindow.hxx"

#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"

#include <cstdio>
#include <stdexcept>
#include <vector>

#include "engine/defines.hxx"

static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT Severity,
                                                    VkDebugUtilsMessageTypeFlagsEXT Type,
                                                    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
                                                    void* pUserData)
{
    printf("Debug callback: %s\n", pCallbackData->pMessage);
    printf("  Severity %s\n", GetDebugSeverityStr(Severity));
    printf("  Type %s\n", GetDebugType(Type));
    printf("  Objects ");
    
    for (uint32_t i = 0; i < pCallbackData->objectCount; i++)
    {
        printf("%llux ", pCallbackData->pObjects[i].objectHandle);
    }

    printf("\n");

    return VK_FALSE;
}

RenderAPIVulkan::RenderAPIVulkan(Window* window) :
RenderAPI(window)
{
	printf("RenderAPIVulkan: initialization\n");
    
    createInstance();
    createDebugCallback();
    setWindow(window);
    mPhysicalDevices.init(mInstance, mWindowSurface);
    mQueueFamily = mPhysicalDevices.selectDevice(VK_QUEUE_GRAPHICS_BIT, true);
    createDevice();
}

RenderAPIVulkan::~RenderAPIVulkan()
{
    vkDestroyDevice(mDevice, nullptr);

    PFN_vkDestroySurfaceKHR vkDestroySurface = VK_NULL_HANDLE;
    vkDestroySurface = (PFN_vkDestroySurfaceKHR)vkGetInstanceProcAddr(mInstance, "vkDestroySurfaceKHR");
    if (!vkDestroySurface)
    {
        printf("RenderAPIVulkan: Cannot find address of vkDestroySurfaceKHR\n");
    }
    vkDestroySurface(mInstance, mWindowSurface, nullptr);
    
    PFN_vkDestroyDebugUtilsMessengerEXT vkDestroyDebugUtilsMessenger = VK_NULL_HANDLE;
    vkDestroyDebugUtilsMessenger = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(mInstance, "vkDestroyDebugUtilsMessengerEXT");
    if (!vkDestroyDebugUtilsMessenger) 
    {
        printf("RenderAPIVulkan: Cannot find address of vkDestroyDebugUtilsMessengerEXT\n");
    }
    vkDestroyDebugUtilsMessenger(mInstance, mDebugMessenger, nullptr);
    
	vkDestroyInstance(mInstance, nullptr);
}

void RenderAPIVulkan::setWindow(Window* window)
{
	GLFWWindow* glfwWindow = dynamic_cast<GLFWWindow*>(window);
    
    if (glfwCreateWindowSurface(mInstance, glfwWindow->getGLFLWindow(), nullptr, &mWindowSurface))
    {
        throw std::runtime_error("RenderAPIVulkan: Error creating GLFW Window Surface\n");
    }
    printf("RenderAPIVulkan: Window Surface created\n");
}

void RenderAPIVulkan::createDevice()
{
    float queuePriorities[] = { 1.0f };

    VkDeviceQueueCreateInfo queueCreateInfo{};
    queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfo.pNext = nullptr;
    queueCreateInfo.flags = 0;
    queueCreateInfo.queueFamilyIndex = mQueueFamily;
    queueCreateInfo.queueCount = 1;
    queueCreateInfo.pQueuePriorities = &queuePriorities[0];

    std::vector<const char*> deviceExtensions = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME,
        VK_KHR_SHADER_DRAW_PARAMETERS_EXTENSION_NAME
    };

    if (mPhysicalDevices.selected().mFeatures.geometryShader == VK_FALSE)
    {
        throw std::runtime_error("RenderAPIVulkan: Device is not support Geometry Shaders!\n");
    }

    if (mPhysicalDevices.selected().mFeatures.tessellationShader == VK_FALSE)
    {
        throw std::runtime_error("RenderAPIVulkan: Device is not support Tesselation Shaders!\n");
    }


    VkDeviceCreateInfo deviceCreateInfo{};
    deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    deviceCreateInfo.pNext = nullptr;
    deviceCreateInfo.flags = 0;
    deviceCreateInfo.queueCreateInfoCount = 1;
    deviceCreateInfo.pQueueCreateInfos = &queueCreateInfo;
    deviceCreateInfo.enabledLayerCount = 0;             // DEPRECATED!
    deviceCreateInfo.ppEnabledLayerNames = nullptr;     // DEPRECATED!
    deviceCreateInfo.enabledExtensionCount = (uint32_t)deviceExtensions.size();
    deviceCreateInfo.ppEnabledExtensionNames = deviceExtensions.data();;
    deviceCreateInfo.pEnabledFeatures = nullptr;

    VkResult result = vkCreateDevice(mPhysicalDevices.selected().mDevice, &deviceCreateInfo, nullptr, &mDevice);
    if (result != VK_SUCCESS)
    {
        throw std::runtime_error("RenderAPIVulkan: Creating Device problem\n");
    }
    printf("RenderAPIVulkan: Device Created\n");
}

void RenderAPIVulkan::createInstance()
{
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

    uint32_t enableExtensionCount = glfwExtensionCount + 1;
    
#if CURRENT_PLATFORM == PLATFORM_MAC
    enableExtensionCount += 2;
#endif
    
    const char** ppEnableExtensionNames = (const char**)malloc(enableExtensionCount * sizeof(char*));
    
    uint32_t offset = 0;
    for (; offset < glfwExtensionCount; offset++)
    {
        ppEnableExtensionNames[offset] = glfwExtensions[offset];
    }
    
    ppEnableExtensionNames[offset++] = VK_EXT_DEBUG_UTILS_EXTENSION_NAME;
    
#if CURRENT_PLATFORM == PLATFORM_MAC
    ppEnableExtensionNames[offset++] = VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME;
    ppEnableExtensionNames[offset++] = VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME;
#endif
    createInfo.enabledExtensionCount = enableExtensionCount;
    createInfo.ppEnabledExtensionNames = ppEnableExtensionNames;
    
    std::vector<const char*> layers = 
    {
        "VK_LAYER_KHRONOS_validation"
    };
    
    createInfo.enabledLayerCount = static_cast<uint32_t>(layers.size());
    createInfo.ppEnabledLayerNames = layers.data();
#if CURRENT_PLATFORM == PLATFORM_MAC
    createInfo.flags = VkInstanceCreateFlagBits::VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
#endif

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

void RenderAPIVulkan::createDebugCallback()
{
    VkDebugUtilsMessengerCreateInfoEXT messengerCreateInfo{};
    messengerCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    messengerCreateInfo.pNext = nullptr;
    messengerCreateInfo.messageSeverity =   VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
                                            VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT |
                                            VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                                            VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    messengerCreateInfo.messageType =   VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                                        VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT|
                                        VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    messengerCreateInfo.pfnUserCallback = &DebugCallback;
    messengerCreateInfo.pUserData = nullptr;
    
    PFN_vkCreateDebugUtilsMessengerEXT vkCreateDebugUtilsMessenger = VK_NULL_HANDLE;
    vkCreateDebugUtilsMessenger = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(mInstance, "vkCreateDebugUtilsMessengerEXT");
    if (!vkCreateDebugUtilsMessenger)
    {
        throw std::runtime_error("RenderAPIVulkan: Cannot find address of VkDebugUtilsMessengerEXT\n");
    }
    
    VkResult res = vkCreateDebugUtilsMessenger(mInstance, &messengerCreateInfo, nullptr, &mDebugMessenger);
    if (res == VK_SUCCESS)
    {
        printf("RenderAPIVulkan: Debug Utills Messager created\n");
    }
    else
    {
        throw std::runtime_error("RenderAPIVulkan: Debug Utills Messager not created\n");
    }
}
