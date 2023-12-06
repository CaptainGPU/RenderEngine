//
// Created by Eugene Karpenko @ CaptainGPU
// https://twitter.com/CaptainGPU
//

#pragma once

#include "window.hxx"

// Application base class
// An application has three main life stages:
// initStage() - initialization stage
// workStage() - the stage of work in which the object logic is updated and the frame is rendered
// finalStage() - the stage when the application ends, in which the resources occupied by the engine components are released

class Application
{
public:
	Application(int width, int height);

	// Application startup method
	void run();

// Emscripten Loop code, see implementation
#if CURRENT_PLATFORM == PLATFORM_EMSCRIPTEN
	static void emsk_loop();
	static Application* app;
	void emsk_workStage();
#endif

private:
	// Initialization stage
	void initStage();
	// The Stage of work in which the object logic is updated and the frame is rendered
	void workStage();
	// The stage when the application ends, in which the resources occupied by the engine components are released
	void finalStage();

private:
	// The concept of a window in an application acts as a display/screen/window in which the scene will be displayed.
	Window* m_window = nullptr;

	// Height and width parameters used to initialize the Window
	int m_width;
	int m_height;
};