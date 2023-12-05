#include "window.hxx"
#include "render.hxx"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "renderGUI.hxx"

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

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	glfwMakeContextCurrent(m_window);

	glfwGetFramebufferSize(m_window, &m_bufferWidth, &m_bufferHeight);

	int version = gladLoadGL(glfwGetProcAddress);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(m_window, true);
	ImGui_ImplOpenGL3_Init("#version 330 core");
	ImGui::StyleColorsClassic();

	Render::init();

	Render::setViewport(0, 0, m_bufferWidth, m_bufferHeight);
}

void Window::frame()
{
	glfwPollEvents();

	Render::draw();

	RenderGUI::starRender();
	RenderGUI::frame();
	RenderGUI::endRender();

	glfwSwapBuffers(m_window);
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
