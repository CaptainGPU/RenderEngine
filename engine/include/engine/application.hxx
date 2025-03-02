#pragma once

#include <cstdint>

#include "engine/window/window.hxx"

void startApplication();

class EngineApplication
{
public:
    EngineApplication(uint32_t widht, uint32_t height, bool isSeparateSimulation);
    
public:
    void run();
    
public:
    void simulate();
    
private:
    void initStage();
    void workStage();
    void finalStage();
    
private:
    Window* mWindow = nullptr;
    uint32_t mWidht = 0;
    uint32_t mHeight = 0;
};
