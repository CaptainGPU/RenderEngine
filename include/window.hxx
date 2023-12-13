//
// Created by Eugene Karpenko @ CaptainGPU
// https://twitter.com/CaptainGPU
//

#pragma once

#include "defines.hxx"

#if (CURRENT_PLATFORM == PLATFORM_WIN) || (CURRENT_PLATFORM == PLATFORM_MAC)
#include "glad/gl.h"
#endif

#if CURRENT_PLATFORM == PLATFORM_EMSCRIPTEN
#include <emscripten.h>
#include <GLES3/gl3.h>
#endif

#include <GLFW/glfw3.h>

// The Window class, a container for displaying a scene frame, is used in the Application.
// For platforms with windowed mode, it is a window, for other platforms it is a display
class Window
{
public:
	Window(int width, int height);

	void initInput();

	// Window update function
	void frame();
    
    void swap();
	
	// The function allows you to find out whether the window has been closed,
	// used to force the application to exit Application::workingStage
	bool shouldClose();

	// The function of closing the window, which releases resources
	void close();

private:
	// Window width and height
	int m_width;
	int m_height;

	// Window validity flag,
	bool m_isValid;

	// GLFW Window Implementation
	GLFWwindow* m_window = nullptr;

	// Window framebuffer width and height
	GLint m_bufferWidth;
	GLint m_bufferHeight;

	static void HandleKeys(GLFWwindow* window, int key, int code, int action, int mode);
};
