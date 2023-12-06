//
// Created by Eugene Karpenko @ CaptainGPU
// https://twitter.com/CaptainGPU
//

#include "application.hxx"

#include <iostream>

// Part of Emscripten Loop code, see header
#if CURRENT_PLATFORM == PLATFORM_EMSCRIPTEN
Application* Application::app = nullptr;
#endif

Application::Application(int width, int height)
{
	printf("Hello, i am Application!\n");

	// Set Width and Height of Application
	// In most cases this is the width and height of the screen/window in which the scene will be displayed
	m_width = width;
	m_height = height;

	// Part of Emscripten Loop code, see header
	#if CURRENT_PLATFORM == PLATFORM_EMSCRIPTEN
	app = this;
	#endif
}

// Initialization stage
void Application::initStage()
{
	printf("Application initialize\n");

	// Initializing the Window component
	m_window = new Window(m_width, m_height);
}

// Part of Emscripten Loop code
// On the Emscripten platform, the window update loop is not available, 
// instead a static callback is used, which is exposed
// on the Emscripten application loop

#if CURRENT_PLATFORM == PLATFORM_EMSCRIPTEN
void Application::emsk_workStage()
{
	m_window->frame();
}

void Application::emsk_loop()
{
	app->emsk_workStage();
}
#endif

// The stage of work in which the object logic is updated and the frame is rendered
void Application::workStage()
{
	printf("Application work\n");

// Part of Emscripten Loop code
#if CURRENT_PLATFORM == PLATFORM_EMSCRIPTEN
	emscripten_set_main_loop(Application::emsk_loop, 0, true);
#else
	while (!m_window->shouldClose())
	{
		m_window->frame();
	}
#endif
}

// The stage when the application ends, in which the resources occupied by the engine components are released
void Application::finalStage()
{
	// Clearing resources occupied by the Window
	m_window->close();
	delete m_window;

	printf("Application say goodbye!\n");
}

// Application startup method
void  Application::run()
{
	// Launch all application life cycles
	initStage();
	workStage();
	finalStage();
}
