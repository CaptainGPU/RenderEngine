#include "engine/graphics/backends/vulkan/vulkanUtils.hxx"

#include <sstream>
#include <string>

const char* GetDebugSeverityStr(VkDebugUtilsMessageSeverityFlagBitsEXT Severity)
{
    switch (Severity) {
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
        return "Verbose";

    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
        return "Info";

    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
        return "Warning";

    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
        return "Error";

    default:
        std::stringstream ss;
        ss << "RenderAPIVulkan: Invalid severity code " << Severity << "\n";
        throw std::runtime_error(ss.str());
    }

    return "NO SUCH SEVERITY!";
}

const char* GetDebugType(VkDebugUtilsMessageTypeFlagsEXT Type)
{
    switch (Type) {
    case VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT:
        return "General";

    case VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT:
        return "Validation";

    case VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT:
        return "Performance";

#ifdef _WIN64 // doesn't work on my Linux for some reason
    case VK_DEBUG_UTILS_MESSAGE_TYPE_DEVICE_ADDRESS_BINDING_BIT_EXT:
        return "Device address binding";
#endif

    default:
            std::stringstream ss;
            ss << "RenderAPIVulkan: Invalid type code " << Type << "\n";
            throw std::runtime_error(ss.str());
    }

    return "NO SUCH TYPE!";
}


void printImageUsageFlags(const VkImageUsageFlags& flags)
{
    if (flags & VK_IMAGE_USAGE_TRANSFER_SRC_BIT) {
        printf("VulkanPhysicalDevices: Image usage transfer src is supported\n");
    }

    if (flags & VK_IMAGE_USAGE_TRANSFER_DST_BIT) {
        printf("VulkanPhysicalDevices: Image usage transfer dest is supported\n");
    }

    if (flags & VK_IMAGE_USAGE_SAMPLED_BIT) {
        printf("VulkanPhysicalDevices: Image usage sampled is supported\n");
    }

    if (flags & VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT) {
        printf("VulkanPhysicalDevices: Image usage color attachment is supported\n");
    }

    if (flags & VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT) {
        printf("VulkanPhysicalDevices: Image usage depth stencil attachment is supported\n");
    }

    if (flags & VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT) {
        printf("VulkanPhysicalDevices: Image usage transient attachment is supported\n");
    }

    if (flags & VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT) {
        printf("VulkanPhysicalDevices: Image usage input attachment is supported\n");
    }
}

uint32_t chooseNumImages(const VkSurfaceCapabilitiesKHR& capabilities)
{
    uint32_t requestedNumImages = capabilities.minImageCount + 1;

    int finalNumImages = 0;

    if ((capabilities.maxImageCount > 0) && (requestedNumImages > capabilities.maxImageCount))
    {
        finalNumImages = capabilities.maxImageCount;
    }
    else
    {
        finalNumImages = requestedNumImages;
    }

    return finalNumImages;
}

VkPresentModeKHR choosePresentMode(const std::vector<VkPresentModeKHR>& presentModes)
{
    for (size_t i = 0; i < presentModes.size(); i++)
    {
        if (presentModes[i] == VK_PRESENT_MODE_MAILBOX_KHR)
        {
            return presentModes[i];
        }
    }

    return VK_PRESENT_MODE_FIFO_KHR;
}

VkSurfaceFormatKHR chooseSurfaceFormatAndColorSpace(const std::vector<VkSurfaceFormatKHR>& surfaceFormats)
{
    for (size_t i = 0; i < surfaceFormats.size(); i++)
    {
        if ((surfaceFormats[i].format == VK_FORMAT_B8G8R8A8_UNORM) && (surfaceFormats[i].colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR))
        {
            return surfaceFormats[i];
        }
    }

    return surfaceFormats[0];
}

VkImageView CreateImageView(VkDevice device, VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, VkImageViewType viewType, uint32_t layerCount, uint32_t mipLevels)
{
    VkImageViewCreateInfo viewInfo{};
    viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    viewInfo.pNext = nullptr;
    viewInfo.flags = 0;
    viewInfo.image = image;
    viewInfo.viewType = viewType;
    viewInfo.format = format;
    viewInfo.components = {};
    viewInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
    viewInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
    viewInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
    viewInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
    viewInfo.subresourceRange = {};
    viewInfo.subresourceRange.aspectMask = aspectFlags;
    viewInfo.subresourceRange.baseMipLevel = 0;
    viewInfo.subresourceRange.levelCount = mipLevels;
    viewInfo.subresourceRange.baseArrayLayer = 0;
    viewInfo.subresourceRange.layerCount = layerCount;

    VkImageView ImageView;

    VkResult result = vkCreateImageView(device, &viewInfo, nullptr, &ImageView);
    if (result != VK_SUCCESS)
    {
        throw std::runtime_error("RenderAPIVulkan: Create ImageView problem\n");
    }

    return ImageView;
}

VkSemaphore createSemaphore(VkDevice device)
{
    VkSemaphoreCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    createInfo.pNext = nullptr;
    createInfo.flags = 0;
    
    VkSemaphore semaphore;
    VkResult result = vkCreateSemaphore(device, &createInfo, nullptr, &semaphore);
    if (result != VK_SUCCESS)
    {
        throw std::runtime_error("RenderAPIVulkan: Create Semaphore problem\n");
    }
    return semaphore;
}
