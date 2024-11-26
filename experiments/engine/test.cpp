#include "engine/test.hpp"

#include <iostream>

#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

GLFWwindow* window = nullptr;

void GLFW_KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if ((key == GLFW_KEY_ESCAPE) && (action == GLFW_PRESS))
	{
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
}

void testEngine()
{
	if (!glfwInit())
	{
		printf("glfwInit fail\n");
		return;
	}

	if (!glfwVulkanSupported())
	{
		printf("glfwVulkanSupported fail\n");
		return;
	}

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Render Engine", nullptr, nullptr);

	if (!window)
	{
		printf("glfwCreateWindow fail\n");
		glfwTerminate();
		return;
	}

	glfwSetKeyCallback(window, GLFW_KeyCallback);

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
	}

	glfwTerminate();
}