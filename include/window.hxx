#pragma once

#include <GLFW/glfw3.h>


class Window
{
public:
	Window(int width, int height);
	void frame();
	bool shouldClose();
	void close();

private:
    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;

	int m_width;
	int m_height;
	bool m_isValid;

	GLFWwindow* m_window = nullptr;
};
