#pragma once

#define GLFW_INCLUDE_NONE
//#include <GLFW/glfw3.h>
#include "glad/gl.h"
#include <GLFW/glfw3.h>


class Window
{
public:
	Window(int width, int height);
	void frame();
	bool shouldClose();
	void close();

private:
	int m_width;
	int m_height;
	bool m_isValid;

	GLFWwindow* m_window = nullptr;

	GLint m_bufferWidth;
	GLint m_bufferHeight;
};
