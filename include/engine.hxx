//
// Created by Eugene Karpenko @ CaptainGPU
// https://twitter.com/CaptainGPU
//
#pragma once

#include <chrono>

#include "sceneManager.hxx"
#include "renderEngine.hxx"

class Engine
{
public:
    Engine();
    
    void init();
    
    void run();
    
    void simulate();
    
    void finish();
    
    void renderStats();
    
    SceneManager* getSceneManager();
    
    static void registreEngine(Engine* engine);

    float getGameTime();

    void setWindowBufferSize(unsigned int widht, unsigned int height);
    void getWindowBufferSize(unsigned int& widht, unsigned int& height);
    
private:
    void calculateDeltaTime();
    void calculateAverage();
    
private:
    SceneManager* m_sceneManager = nullptr;
    RenderEngine* m_renderEngine = nullptr;
    
    float m_deltaTime;
    std::chrono::steady_clock::time_point m_lastTime;
    float m_gameTime;
    
    float m_all;
    float m_avr;
    float m_count;

    unsigned int m_windowFrameBufferWidth;
    unsigned int m_windowFrameBufferHeight;
    
    static Engine* g_Engine;
    
public:
    static Engine* get();
};
