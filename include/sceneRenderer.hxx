//
// Created by Eugene Karpenko @ CaptainGPU
// https://twitter.com/CaptainGPU
//

#pragma once

#include "renderer.hxx"
#include "uniform.hxx"

enum SceneRendererPasses
{
    BASE_PASS,
    BOUND_PASS,
    
    PASS_COUNT
};

class SceneRenderer : public Renderer
{
public:
    SceneRenderer();
    void render(RenderInfo& renderInfo) override;
    void init() override;
    void finish() override;

    void drawDebugUI() override;

private:
    void renderBasePass(RenderInfo& renderInfo);
    void renderBoundPass(RenderInfo& renderInfo);
    
private:
    bool m_renderBasePass;
    bool m_renderBoundPass;

    glm::vec3 m_boundColor;

    Uniform* m_matrixModelUniform;
    Uniform* m_matrixViewUniform;
    Uniform* m_matrixProjectionUniform;
    Uniform* m_timeUniform;

    Uniform* m_boundMatrixModelUniform;
    Uniform* m_boundMatrixViewUniform;
    Uniform* m_boundMatrixProjectionUniform;
    Uniform* m_boundColorUniform;
private:
};
