//
// Created by Eugene Karpenko @ CaptainGPU
// https://twitter.com/CaptainGPU
//
#pragma once

#include "renderer.hxx"

class RenderEngine
{
public:
    void init();
    void render();
    void finish();
private:
    Renderer* m_renderer;
};
