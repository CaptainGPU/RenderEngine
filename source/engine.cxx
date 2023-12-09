//
// Created by Eugene Karpenko @ CaptainGPU
// https://twitter.com/CaptainGPU
//
#include "engine.hxx"
#include "renderGUI.hxx"
#include <GLFW/glfw3.h>

#include <iostream>

Engine* Engine::g_Engine;

Engine::Engine()
{
    m_deltaTime = .0;
    g_Engine = this;
}

void Engine::init()
{
    m_sceneManager = new SceneManager();
    
    m_renderEngine = new RenderEngine();
    m_renderEngine->init();
}

void Engine::run()
{
    m_sceneManager->run();
}


void Engine::finish()
{
    m_renderEngine->finish();
    delete m_renderEngine;
    m_renderEngine = nullptr;
    
    
    m_sceneManager->finish();
    delete m_sceneManager;
    m_sceneManager = nullptr;
}

void Engine::calculateDeltaTime()
{
    static auto startTime = std::chrono::high_resolution_clock::now();
    auto currentTime = std::chrono::high_resolution_clock::now();
    
    
    m_gameTime = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();
    m_deltaTime = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - m_lastTime).count();
    
    m_lastTime = currentTime;
}

void Engine::simulate()
{
    calculateDeltaTime();
    
    m_sceneManager->simulate(m_deltaTime);
    
    m_renderEngine->render();

    // Render UI
    RenderGUI::starRender();
    m_sceneManager->drawGUI();
    //RenderGUI::frame();
    RenderGUI::endRender();

}

void Engine::registreEngine(Engine* engine)
{
    g_Engine = engine;
}

SceneManager* Engine::getSceneManager()
{
    return m_sceneManager;
}

Engine* Engine::get()
{
    return g_Engine;
}
