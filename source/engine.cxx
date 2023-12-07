//
// Created by Eugene Karpenko @ CaptainGPU
// https://twitter.com/CaptainGPU
//
#include "engine.hxx"

#include <chrono>
#include <iostream>

Engine::Engine()
{
    m_deltaTime = .0;
}

void Engine::init()
{
    m_sceneManager = new SceneManager();
}

void Engine::finish()
{
    m_sceneManager->finish();
    delete m_sceneManager;
}

void Engine::calculateDeltaTime()
{
    static auto startTime = std::chrono::high_resolution_clock::now();
    auto currentTime = std::chrono::high_resolution_clock::now();
    m_deltaTime = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();
}

void Engine::simulate()
{
    calculateDeltaTime();
    
    m_sceneManager->simulate(m_deltaTime);
}
