#pragma once

#include "window.hxx"

class Application
{
public:
	Application(int width, int height);
	void run();

private:
	void initStage();
	void workStage();
	void finalStage();

private:
    Application(const Application&) = delete;
    Application& operator=(const Application&) = delete;

	Window* m_window = nullptr;

	int m_width;
	int m_height;
};