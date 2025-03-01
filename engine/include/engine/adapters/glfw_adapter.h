#pragma once

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_COCOA
#include <GLFW/glfw3native.h>
#include <QuartzCore/QuartzCore.hpp>
#include <AppKit/AppKit.hpp>

NS::Window* get_ns_window(GLFWwindow* glfwWindow, CA::MetalLayer* layer);
