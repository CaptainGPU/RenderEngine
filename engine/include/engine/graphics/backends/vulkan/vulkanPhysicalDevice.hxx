#pragma once

#include <vulkan/vulkan.h>
#include <vector>

struct PhysicalDevice
{
    VkPhysicalDevice mDevice;
    VkPhysicalDeviceProperties mProperties;
    std::vector<VkQueueFamilyProperties> mFamilyProperties;
    std::vector<VkBool32> mQueueSupportPresents;
    std::vector<VkSurfaceFormatKHR> mSurfaceFormats;
    VkSurfaceCapabilitiesKHR mSurfaceCapabilities;
    VkPhysicalDeviceMemoryProperties mMemoryProperties;
    std::vector<VkPresentModeKHR> mPresentModes;
    VkPhysicalDeviceFeatures mFeatures;
};

class VulkanPhysicalDevices
{
public:
    VulkanPhysicalDevices();
    ~VulkanPhysicalDevices();
    
    void init(const VkInstance& instance, const VkSurfaceKHR& surface);
    int32_t selectDevice(VkQueueFlags requiredQueueType, bool supportsPresents);
    const PhysicalDevice& selected() const;
private:
    std::vector<PhysicalDevice> mDevices;
    int32_t mDeviceIndex = -1;
};
