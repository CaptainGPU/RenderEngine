#pragma once

#include <vulkan/vulkan.h>
#include <vector>

const char* GetDebugSeverityStr(VkDebugUtilsMessageSeverityFlagBitsEXT Severity);
const char* GetDebugType(VkDebugUtilsMessageTypeFlagsEXT Type);
void printImageUsageFlags(const VkImageUsageFlags& flags);
uint32_t chooseNumImages(const VkSurfaceCapabilitiesKHR& capabilities);
VkPresentModeKHR choosePresentMode(const std::vector<VkPresentModeKHR>& presentModes);
VkSurfaceFormatKHR chooseSurfaceFormatAndColorSpace(const std::vector<VkSurfaceFormatKHR>& surfaceFormats);
VkImageView CreateImageView(VkDevice device, VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, VkImageViewType viewType, uint32_t layerCount, uint32_t mipLevels);