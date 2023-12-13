//
// Created by Eugene Karpenko @ CaptainGPU
// https://twitter.com/CaptainGPU
//

#include "window.hxx"
#include "render.hxx"
#include "input.hxx"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

Window::Window(int width, int height)
{
	m_width = width;
	m_height = height;

	m_isValid = true;

	// GLFW Initialization
	if (!glfwInit())
	{
		// If GLFW cannot be initialized, the window is invalid
		m_isValid = false;
	}
    
    // OpenGL GLFW context settings
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	// Creating GLFW Window
	m_window = glfwCreateWindow(m_width, m_height, "Промінь", nullptr, nullptr);

	if (!m_window)
	{
		// If it is impossible to create a GLFW window, the window is invalid
		m_isValid = false;
	}

	glfwMakeContextCurrent(m_window);

	// Getting framebuffer sizes
	glfwGetFramebufferSize(m_window, &m_bufferWidth, &m_bufferHeight);

	// GLAD initialization on WIN platform
#if (CURRENT_PLATFORM == PLATFORM_WIN || CURRENT_PLATFORM == PLATFORM_MAC)
	int version = gladLoadGL(glfwGetProcAddress);
#endif

	// ImGUI Initialization
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(m_window, true);
#if (CURRENT_PLATFORM == PLATFORM_WIN || CURRENT_PLATFORM == PLATFORM_MAC)
	ImGui_ImplOpenGL3_Init("#version 330 core");
#else
	ImGui_ImplOpenGL3_Init("#version 300 es");
#endif
	ImGui::StyleColorsClassic();

	// Render Initialization
	//Render::init();

	// Set viewport to Window framebuffer size
	Render::setViewport(0, 0, m_bufferWidth, m_bufferHeight);
}

void Window::initInput()
{
	Input::init();
	glfwSetKeyCallback(m_window, HandleKeys);
}

// Window update function
void Window::frame()
{
	// GLFW event handling
	glfwPollEvents();
}

void Window::swap()
{
    // Swap Window framebuffer
    glfwSwapBuffers(m_window);
}

// The function allows you to find out whether the window has been closed,
// used to force the application to exit Application::workingStage
bool Window::shouldClose()
{
	return !m_isValid || glfwWindowShouldClose(m_window);
}

// The function of closing the window, which releases resources
void Window::close()
{
	if (m_isValid && m_window)
	{
		glfwDestroyWindow(m_window);
	}

	glfwTerminate();
}

void Window::HandleKeys(GLFWwindow* window, int key, int code, int action, int mode)
{
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key >= 0 && key < 1024)
	{
		KEYS engineKey = KEYS::KEY_COUNT;
		bool registerKey = false;

		if (key == GLFW_KEY_W)
		{
			engineKey = KEYS::KEY_W;
			registerKey = true;
		}

		if (key == GLFW_KEY_S)
		{
			engineKey = KEYS::KEY_S;
			registerKey = true;
		}

		if (key == GLFW_KEY_A)
		{
			engineKey = KEYS::KEY_A;
			registerKey = true;
		}

		if (key == GLFW_KEY_D)
		{
			engineKey = KEYS::KEY_D;
			registerKey = true;
		}

		if (key == GLFW_KEY_Q)
		{
			engineKey = KEYS::KEY_Q;
			registerKey = true;
		}

		if (key == GLFW_KEY_E)
		{
			engineKey = KEYS::KEY_E;
			registerKey = true;
		}

		if (registerKey)
		{
			if (action == GLFW_PRESS)
			{
				Input::setKeyPressed(engineKey, true);
			}
			else if(action == GLFW_RELEASE)
			{
				Input::setKeyPressed(engineKey, false);
			}
		}
	}
	else
	{
		return;
	}
}
