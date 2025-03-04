#pragma once

#include <vulkan/vulkan.h>

const char* GetDebugSeverityStr(VkDebugUtilsMessageSeverityFlagBitsEXT Severity);
const char* GetDebugType(VkDebugUtilsMessageTypeFlagsEXT Type);
void printImageUsageFlags(const VkImageUsageFlags& flags);
