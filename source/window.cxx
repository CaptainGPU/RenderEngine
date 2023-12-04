#include "window.hxx"

Window::Window(int width, int height)
{
	m_width = width;
	m_height = height;

	m_isValid = true;

	if (!glfwInit())
	{
		m_isValid = false;
	}

	m_window = glfwCreateWindow(m_width, m_height, "Промінь", nullptr, nullptr);

	if (!m_window)
	{
		m_isValid = false;
	}

	glfwMakeContextCurrent(m_window);
}

void Window::frame()
{
	glfwPollEvents();
}

bool Window::shouldClose()
{
	return !m_isValid || glfwWindowShouldClose(m_window);
}

void Window::close()
{
	if (m_isValid && m_window)
	{
		glfwDestroyWindow(m_window);
	}

	glfwTerminate();
}
