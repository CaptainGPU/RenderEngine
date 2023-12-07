//
// Created by Eugene Karpenko @ CaptainGPU
// https://twitter.com/CaptainGPU
//
#pragma once

#include "sceneManager.hxx"

class Engine
{
public:
    Engine();
    
    void init();
    
    void simulate();
    
    void finish();
    
private:
    void calculateDeltaTime();
    
private:
    SceneManager* m_sceneManager = nullptr;
    
    float m_deltaTime;
};
