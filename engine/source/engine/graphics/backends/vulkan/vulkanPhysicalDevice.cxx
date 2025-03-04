#include "engine/graphics/backends/vulkan/vulkanPhysicalDevice.hxx"
#include "engine/graphics/backends/vulkan/vulkanUtils.hxx"

#include <stdexcept>

VulkanPhysicalDevices::VulkanPhysicalDevices()
{
}

VulkanPhysicalDevices::~VulkanPhysicalDevices()
{
}

void VulkanPhysicalDevices::init(const VkInstance& instance, const VkSurfaceKHR& surface)
{
    uint32_t numDevices = 0;
    
    VkResult result = vkEnumeratePhysicalDevices(instance, &numDevices, nullptr);
    if (result != VK_SUCCESS)
    {
        throw std::runtime_error("VulkanPhysicalDevices: Not find physical devices\n");
    }
    
    printf("VulkanPhysicalDevices: Num physical devices: %d\n", numDevices);
    mDevices.resize(numDevices);
    
    std::vector<VkPhysicalDevice> devices(numDevices);
    
    result = vkEnumeratePhysicalDevices(instance, &numDevices, devices.data());
    if (result != VK_SUCCESS)
    {
        throw std::runtime_error("VulkanPhysicalDevices: Not find physical devices\n");
    }
    
    for (uint32_t i = 0; i < numDevices; i++)
    {
        VkPhysicalDevice physDevice = devices[i];
        mDevices[i].mDevice = physDevice;
        
        vkGetPhysicalDeviceProperties(physDevice, &mDevices[i].mProperties);
        
        printf("VulkanPhysicalDevices: Device name: %s\n", mDevices[i].mProperties.deviceName);
        uint32_t apiVersion = mDevices[i].mProperties.apiVersion;
        printf("VulkanPhysicalDevices: API Version %d.%d.%d.%d\n",
               VK_API_VERSION_VARIANT(apiVersion),
               VK_API_VERSION_MAJOR(apiVersion),
               VK_API_VERSION_MINOR(apiVersion),
               VK_API_VERSION_PATCH(apiVersion));
        
        uint32_t numQueueFamilies = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(physDevice, &numQueueFamilies, nullptr);
        printf("VulkanPhysicalDevices: Num of family queues: %d\n", numQueueFamilies);

        mDevices[i].mFamilyProperties.resize(numQueueFamilies);
        mDevices[i].mQueueSupportPresents.resize(numQueueFamilies);

        vkGetPhysicalDeviceQueueFamilyProperties(physDevice, &numQueueFamilies, mDevices[i].mFamilyProperties.data());

        for (uint32_t q = 0; q < numQueueFamilies; q++)
        {
            const VkQueueFamilyProperties& queueFamilyProperties = mDevices[i].mFamilyProperties[q];

            printf("VulkanPhysicalDevices: Family %d num queues: %d\n", q, queueFamilyProperties.queueCount);
            VkQueueFlags flags = queueFamilyProperties.queueFlags;
            printf("VulkanPhysicalDevices: GFX%s, Compute: %s, Transfer: %s, Sparse bindings: %s\n",
                (flags & VK_QUEUE_GRAPHICS_BIT) ? "Yes" : "No",
                (flags & VK_QUEUE_COMPUTE_BIT) ? "Yes" : "No",
                (flags & VK_QUEUE_TRANSFER_BIT) ? "Yes" : "No",
                (flags & VK_QUEUE_SPARSE_BINDING_BIT) ? "Yes" : "No");

            result = vkGetPhysicalDeviceSurfaceSupportKHR(physDevice, q, surface, &(mDevices[i].mQueueSupportPresents[q]));
            if (result != VK_SUCCESS)
            {
                throw std::runtime_error("VulkanPhysicalDevices: Physical Device Surface support error\n");
            }
        }

        uint32_t numFormats = 0;

        result = vkGetPhysicalDeviceSurfaceFormatsKHR(physDevice, surface, &numFormats, nullptr);
        if (result != VK_SUCCESS || !(numFormats > 0))
        {
            throw std::runtime_error("VulkanPhysicalDevices: Physical Device Formats support error\n");
        }

        mDevices[i].mSurfaceFormats.resize(numFormats);
        result = vkGetPhysicalDeviceSurfaceFormatsKHR(physDevice, surface, &numFormats, mDevices[i].mSurfaceFormats.data());
        if (result != VK_SUCCESS)
        {
            throw std::runtime_error("VulkanPhysicalDevices: Physical Device Formats support error\n");
        }

        for (size_t j = 0; j < numFormats; j++)
        {
            const VkSurfaceFormatKHR& surfaceFormat = mDevices[i].mSurfaceFormats[j];
            printf("VulkanPhysicalDevices: Format % x color space %x\n", surfaceFormat.format, surfaceFormat.colorSpace);
        }

        result = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physDevice, surface, &(mDevices[i].mSurfaceCapabilities));
        if (result != VK_SUCCESS)
        {
            throw std::runtime_error("VulkanPhysicalDevices: Physical Device Surface Capabilities error\n");
        }
        printImageUsageFlags(mDevices[i].mSurfaceCapabilities.supportedUsageFlags);

        uint32_t numPresentModes = 0;
        result = vkGetPhysicalDeviceSurfacePresentModesKHR(physDevice, surface, &numPresentModes, nullptr);
        if (result != VK_SUCCESS)
        {
            throw std::runtime_error("VulkanPhysicalDevices: Physical Device Surface present modes error\n");
        }

        printf("VulkanPhysicalDevices: Num of present modes: %d\n", numPresentModes);
        mDevices[i].mPresentModes.resize(numPresentModes);

        result = vkGetPhysicalDeviceSurfacePresentModesKHR(physDevice, surface, &numPresentModes, mDevices[i].mPresentModes.data());
        if (result != VK_SUCCESS)
        {
            throw std::runtime_error("VulkanPhysicalDevices: Physical Device Surface present modes error\n");
        }

        vkGetPhysicalDeviceMemoryProperties(physDevice, &(mDevices[i].mMemoryProperties));

        printf("VulkanPhysicalDevices: Num memory types: %d\n", mDevices[i].mMemoryProperties.memoryTypeCount);
        for (uint32_t j = 0; j < mDevices[i].mMemoryProperties.memoryTypeCount; j++)
        {
            printf("VulkanPhysicalDevices: Memory Type%d: flags: %x heap %x\n",
                j,
                mDevices[i].mMemoryProperties.memoryTypes[j].propertyFlags,
                mDevices[i].mMemoryProperties.memoryTypes[j].heapIndex);
        }

        printf("VulkanPhysicalDevices: Num heap types %d\n", mDevices[i].mMemoryProperties.memoryHeapCount);

    }
}
