#include "engine/defines.hxx"

#include <iostream>
#include <chrono>
#include <thread>

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
    
    mWindow = new GLFWWindow(mWidht, mHeight);

    Render::initRenderAPI();
    
    Render::get()->setWindow(mWindow);
}

void EngineApplication::workStage()
{
    printf("EngineApplication: working\n");
    
    const int targetFPS = 60; // TODO: Implement RenderEngine config target FPS parameter
    
    const std::chrono::duration<double> targetFrameDuration = std::chrono::duration<double>(1.0 / targetFPS);
    
    while (!mWindow->isShouldClose())
    {
        std::chrono::time_point<std::chrono::high_resolution_clock> frameStart = std::chrono::high_resolution_clock::now();
        
        simulate();
        
        std::chrono::time_point<std::chrono::high_resolution_clock> frameEnd = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::nano> frameDuration = frameEnd - frameStart;
        if (frameDuration < targetFrameDuration) 
        {
            std::this_thread::sleep_for(targetFrameDuration - frameDuration);
        }
    }
}

void EngineApplication::simulate()
{
    mWindow->frame();
    
    Render::get()->renderFrame();
}

void EngineApplication::finalStage()
{
    Render::get()->shootDownRenderAPI();

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
#if CURRENT_PLATFORM == PLATFORM_WIN
    printf("Hello Windows64\n");
#else
    printf("Hello MacOS\n");
#endif

    printf("Start Engine!\n");
    
    EngineApplication* application = new EngineApplication(640, 480, false);
    application->run();
    
    return;
}
