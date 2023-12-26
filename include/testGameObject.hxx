//
// Created by Eugene Karpenko @ CaptainGPU
// https://twitter.com/CaptainGPU
//
#pragma once

#include "gameObject.hxx"

class TestGameObject : public GameObject
{
public:
    TestGameObject(std::string modelName);
    
    void drawGUI() override;
    void update(float deltaTime) override;
    
private:
    float m_deltaTime;
    float m_time;
};
