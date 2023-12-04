#include "application.hxx"

#include <iostream>

Application::Application(int width, int height)
{
	printf("Hello, i am Application!\n");

	m_width = width;
	m_height = height;
}

void Application::initStage()
{
	printf("Application initialize\n");

	m_window = new Window(m_width, m_height);
}

void Application::workStage()
{
	printf("Application work\n");

	while (!m_window->shouldClose())
	{
		m_window->frame();
	}
}

void Application::finalStage()
{
	printf("Application say goodbye!");

	m_window->close();

	delete m_window;
}

void  Application::run()
{
	initStage();

	workStage();

	finalStage();
}
