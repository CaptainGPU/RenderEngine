//
// Created by Eugene Karpenko @ CaptainGPU
// https://twitter.com/CaptainGPU
//
#pragma once

#include "scene.hxx"

class PoinLightGameObject;

class TestScene : public Scene {
    
public:
    TestScene();
    ~TestScene();
    
    void startPlay() override;
    void construct() override;
    
    void update(float deltaTime) override;
    
private:
    float m_time;
    
    std::vector<PoinLightGameObject* > m_lights;
    std::vector<GameObject* > m_meshes;
    GameObject* m_whiteLight;
};
