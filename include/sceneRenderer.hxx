//
// Created by Eugene Karpenko @ CaptainGPU
// https://twitter.com/CaptainGPU
//

#pragma once

#include "renderer.hxx"
#include "renderInfo.hxx"

enum SceneRendererPasses
{
    TRIANGLE_PASS,
    
    PASS_COUNT
};

class SceneRenderer : public Renderer
{
public:
    SceneRenderer();
    void render() override;
    void init() override;
    void finish() override;
private:
};
