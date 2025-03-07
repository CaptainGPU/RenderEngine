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
    createSwapchain();
    createCommandBufferPool();
    createCommandBuffers();
    recordCommandBuffers();
}

RenderAPIVulkan::~RenderAPIVulkan()
{
    for (size_t i = 0; i < mSwapchainImageViews.size(); i++)
    {
        vkDestroyImageView(mDevice, mSwapchainImageViews[i], nullptr);
    }

    vkDestroySwapchainKHR(mDevice, mSwapchain, nullptr);

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
#if CURRENT_PLATFORM == PLATFORM_MAC
        "VK_KHR_portability_subset",
#endif
        VK_KHR_SWAPCHAIN_EXTENSION_NAME,
        VK_KHR_SHADER_DRAW_PARAMETERS_EXTENSION_NAME
    };

#if CURRENT_PLATFORM != PLATFORM_MAC
    if (mPhysicalDevices.selected().mFeatures.geometryShader == VK_FALSE)
    {
        throw std::runtime_error("RenderAPIVulkan: Device is not support Geometry Shaders!\n");
    }
#endif

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

void RenderAPIVulkan::createSwapchain()
{
    const VkSurfaceCapabilitiesKHR& surfaceCapabilities = mPhysicalDevices.selected().mSurfaceCapabilities;

    uint32_t numImages = chooseNumImages(surfaceCapabilities);
    
    const std::vector<VkPresentModeKHR>& presentModes = mPhysicalDevices.selected().mPresentModes;
    VkPresentModeKHR presentMode = choosePresentMode(presentModes);

    mSwapchainSurfaceFormat = chooseSurfaceFormatAndColorSpace(mPhysicalDevices.selected().mSurfaceFormats);

    VkSwapchainCreateInfoKHR swapchainCreateInfo{};
    swapchainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    swapchainCreateInfo.pNext = nullptr;
    swapchainCreateInfo.flags = 0;
    swapchainCreateInfo.surface = mWindowSurface;
    swapchainCreateInfo.minImageCount = numImages;
    swapchainCreateInfo.imageFormat = mSwapchainSurfaceFormat.format;
    swapchainCreateInfo.imageColorSpace = mSwapchainSurfaceFormat.colorSpace;
    swapchainCreateInfo.imageExtent = surfaceCapabilities.currentExtent;
    swapchainCreateInfo.imageArrayLayers = 1;
    swapchainCreateInfo.imageUsage = (VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT);
    swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    swapchainCreateInfo.queueFamilyIndexCount = 1;
    swapchainCreateInfo.pQueueFamilyIndices = &mQueueFamily;
    swapchainCreateInfo.preTransform = surfaceCapabilities.currentTransform;
    swapchainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    swapchainCreateInfo.presentMode = presentMode;
    swapchainCreateInfo.clipped = VK_TRUE;

    VkResult result = vkCreateSwapchainKHR(mDevice, &swapchainCreateInfo, nullptr, &mSwapchain);
    if (result != VK_SUCCESS)
    {
        throw std::runtime_error("RenderAPIVulkan: Creating Swapchain problem\n");
    }
    printf("RenderAPIVulkan: Swapchain created\n");

    uint32_t numSwapchainImages = 0;
    result = vkGetSwapchainImagesKHR(mDevice, mSwapchain, &numSwapchainImages, nullptr);
    if (result != VK_SUCCESS)
    {
        throw std::runtime_error("RenderAPIVulkan: Get Swapchain num images problem\n");
    }

    mNumSwapchainImages = numSwapchainImages;
    printf("RenderAPIVulkan: Num of swapchain images: %d\n", mNumSwapchainImages);
    mSwapchainImages.resize(numSwapchainImages);
    mSwapchainImageViews.resize(numSwapchainImages);

    result = vkGetSwapchainImagesKHR(mDevice, mSwapchain, &numSwapchainImages, mSwapchainImages.data());
    if (result != VK_SUCCESS)
    {
        throw std::runtime_error("RenderAPIVulkan: Get Swapchain images problem\n");
    }

    int32_t layerCount = 1;
    int32_t mipLevels = 1;
    for (uint32_t i = 0; i < numSwapchainImages; i++)
    {
        mSwapchainImageViews[i] = CreateImageView(mDevice, mSwapchainImages[i], mSwapchainSurfaceFormat.format, VK_IMAGE_ASPECT_COLOR_BIT, VK_IMAGE_VIEW_TYPE_2D, layerCount, mipLevels);
    }
    printf("RenderAPIVulkan: Created %d Swapchain ImageViews\n", numSwapchainImages);
}

void RenderAPIVulkan::createCommandBufferPool()
{
    VkCommandPoolCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    createInfo.flags = 0;
    createInfo.queueFamilyIndex = mQueueFamily;

    VkResult result = vkCreateCommandPool(mDevice, &createInfo, nullptr, &mCommandPool);
    if (result != VK_SUCCESS)
    {
        throw std::runtime_error("RenderAPIVulkan: Create Command Pool problem\n");
    }
    printf("RenderAPIVulkan: Command Pool created\n");
}

void RenderAPIVulkan::createCommandBuffers()
{
    mCommandBuffers.resize(mNumSwapchainImages);

    VkCommandBufferAllocateInfo allocateInfo{};
    allocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocateInfo.pNext = nullptr;
    allocateInfo.commandPool = mCommandPool;
    allocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocateInfo.commandBufferCount = mNumSwapchainImages;

    VkResult result = vkAllocateCommandBuffers(mDevice, &allocateInfo, mCommandBuffers.data());
    if (result != VK_SUCCESS)
    {
        throw std::runtime_error("RenderAPIVulkan: Create Command Buffers problem\n");
    }

    printf("RenderAPIVulkan: Created %d Command Buffers\n", mNumSwapchainImages);
}

void RenderAPIVulkan::recordCommandBuffers()
{
    VkClearColorValue clearColor = { 0.0f, 0.25f, 0.5f, 1.0f };

    VkImageSubresourceRange imageRange{};
    imageRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    imageRange.baseMipLevel = 0;
    imageRange.levelCount = 1;
    imageRange.baseArrayLayer = 0;
    imageRange.layerCount = 1;

    for (uint32_t i = 0; i < mCommandBuffers.size(); i++)
    {
        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.pNext = nullptr;
        beginInfo.flags = 0;
        beginInfo.pInheritanceInfo = nullptr;

        VkResult result = vkBeginCommandBuffer(mCommandBuffers[i], &beginInfo);
        if (result != VK_SUCCESS)
        {
            throw std::runtime_error("RenderAPIVulkan: Begin Command Buffers problem\n");
        }

        vkCmdClearColorImage(mCommandBuffers[i], mSwapchainImages[i], VK_IMAGE_LAYOUT_GENERAL, &clearColor, 1, &imageRange);

        result = vkEndCommandBuffer(mCommandBuffers[i]);
        if (result != VK_SUCCESS)
        {
            throw std::runtime_error("RenderAPIVulkan: End Command Buffers problem\n");
        }
    }

    printf("RenderAPIVulkan: Command Buffers recorded\n");
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
