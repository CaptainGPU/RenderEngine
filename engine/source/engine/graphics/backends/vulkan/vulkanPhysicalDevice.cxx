#include "engine/graphics/backends/vulkan/vulkanPhysicalDevice.hxx"

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
        throw std::runtime_error("VulkanPhysicalDevices: Not find physical devices");
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
    }
}
