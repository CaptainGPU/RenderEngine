//
// Created by Eugene Karpenko @ CaptainGPU
// https://twitter.com/CaptainGPU
//
#pragma once

#include "camera.hxx"

class Camera2D : public Camera
{
public:
    Camera2D();

    virtual void update(float deltaTime) override;

private:
    float m_speed;
};
