#pragma once
#include <cstdint>
#include <vulkan/vulkan.h>

#include "engine/graphics/renderAPI.hxx"

class RenderAPIVulkan : public RenderAPI
{
public:
	RenderAPIVulkan();
	~RenderAPIVulkan();

	virtual void setWindow(Window* window);

private:
	VkInstance mInstance;
};