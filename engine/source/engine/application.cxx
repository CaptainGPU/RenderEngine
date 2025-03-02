#include <iostream>

#include "engine/application.hxx"
#include "engine/graphics/render.hxx"

#include "engine/window/backends/glfw/glfwWindow.hxx"

EngineApplication::EngineApplication(uint32_t widht, uint32_t height, bool isSeparateSimulation)
{
    mWidht = widht;
    mHeight = height;
}

void EngineApplication::initStage()
{
    printf("EngineApplication: initialize\n");
    
    Render::initRenderAPI();
    
    mWindow = new GLFWWindow(mWidht, mHeight);
    Render::get()->setWindow(mWindow);
}

void EngineApplication::workStage()
{
    printf("EngineApplication: working\n");
    
    while (!mWindow->isShouldClose())
    {
        simulate();
    }
}

void EngineApplication::simulate()
{
    mWindow->frame();
    
    Render::get()->renderFrame();
}

void EngineApplication::finalStage()
{
    mWindow->close();
    mWindow = nullptr;
}

void EngineApplication::run()
{
    initStage();
    workStage();
    finalStage();
}

void startApplication()
{
    printf("Start Engine!\n");
    
    EngineApplication* application = new EngineApplication(800, 600, false);
    application->run();
    
    return;
}
