//
// Created by Eugene Karpenko @ CaptainGPU
// https://twitter.com/CaptainGPU
//

#pragma once

#include "renderer.hxx"
#include "uniform.hxx"

enum SceneRendererPasses
{
    TRIANGLE_PASS,
    
    PASS_COUNT
};

class SceneRenderer : public Renderer
{
public:
    SceneRenderer();
    void render(RenderInfo& renderInfo) override;
    void init() override;
    void finish() override;
    
private:
    Uniform* m_matrixModelUniform;
    Uniform* m_matrixViewUniform;
    Uniform* m_matrixProjectionUniform;
private:
};
