//
// Created by Eugene Karpenko @ CaptainGPU
// https://twitter.com/CaptainGPU
//

#pragma once

#include "renderer.hxx"

class SceneRenderer : public Renderer
{
public:
    SceneRenderer();
    void render() override;
};
