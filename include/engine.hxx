//
// Created by Eugene Karpenko @ CaptainGPU
// https://twitter.com/CaptainGPU
//
#pragma once

#include <chrono>

#include "sceneManager.hxx"

class Engine
{
public:
    Engine();
    
    void init();
    
    void run();
    
    void simulate();
    
    void finish();
    
    static void registreEngine(Engine* engine);
    
private:
    void calculateDeltaTime();
    
private:
    SceneManager* m_sceneManager = nullptr;
    
    float m_deltaTime;
    std::chrono::steady_clock::time_point m_lastTime;
    float m_gameTime;
    
    static Engine* g_Engine;
};
