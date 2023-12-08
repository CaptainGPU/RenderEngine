//
// Created by Eugene Karpenko @ CaptainGPU
// https://twitter.com/CaptainGPU
//
#pragma once

#include "scene.hxx"

class SceneManager
{
public:
    SceneManager();
    
    void run();
    
    void setScene(Scene* scene);
    
    void simulate(float deltaTime);
    void drawGUI();
    
    void finish();
    
private:
    void releaseScene();
    
    Scene* m_scene = nullptr;
};
