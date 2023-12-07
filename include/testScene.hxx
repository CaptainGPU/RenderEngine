//
// Created by Eugene Karpenko @ CaptainGPU
// https://twitter.com/CaptainGPU
//
#pragma once

#include "scene.hxx"

class TestScene : public Scene {
    
public:
    TestScene();
    ~TestScene();
    
    void startPlay() override;
};
