//
// Created by Eugene Karpenko @ CaptainGPU
// https://twitter.com/CaptainGPU
//
#include "engine.hxx"
#include "renderGUI.hxx"
#include "input.hxx"

#include <GLFW/glfw3.h>

#include <iostream>
#include <imgui.h>

Engine* Engine::g_Engine;

Engine::Engine():
m_all(1.0f),
m_avr(30.0f),
m_count(1.0f)
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
    m_lastTime = std::chrono::high_resolution_clock::now();
    
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

void Engine::renderStats()
{
    ImGui::Begin("Stats");
    ImGui::Text("W/A/S/D/Q/E - camera movement");
    ImGui::Text("T - toogle input");
    if (Input::isInputEnable())
    {
        ImGui::Text("Input: Enable");
    }
    else
    {
        ImGui::Text("Input: Disable");
    }
    ImGui::Text("Time: %f", m_gameTime);
    ImGui::Text("RenderTime: %f(ms)", m_deltaTime);
    ImGui::Text("FPS: %f", 1.0f / m_deltaTime);
    ImGui::Text("FPS(avr): %f", 1.0f / m_avr);
    
    m_renderEngine->renderStats();
    ImGui::End();
}

void Engine::calculateDeltaTime()
{
    static auto startTime = std::chrono::high_resolution_clock::now();
    auto currentTime = std::chrono::high_resolution_clock::now();
    
    
    m_gameTime = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();
    m_deltaTime = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - m_lastTime).count();
    
    m_lastTime = currentTime;
    calculateAverage();
}

void Engine::calculateAverage()
{
    m_all += m_deltaTime;
    m_count += 1.0;
    if (m_all > 1.0)
    {
        m_avr = m_all / m_count;
        m_all = .0f;
        m_count = .0;
    }
}

void Engine::simulate()
{
    calculateDeltaTime();
    
    m_sceneManager->simulate(m_deltaTime);
    
    m_renderEngine->render();

    // Render UI
    RenderGUI::starRender();
    m_sceneManager->drawGUI();
    renderStats();
    m_renderEngine->rendererDrawDebugUI();
    RenderGUI::endRender();

}

void Engine::registreEngine(Engine* engine)
{
    g_Engine = engine;
}

float Engine::getGameTime()
{
    return m_gameTime;
}

SceneManager* Engine::getSceneManager()
{
    return m_sceneManager;
}

Engine* Engine::get()
{
    return g_Engine;
}
