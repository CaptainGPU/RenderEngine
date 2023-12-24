//
// Created by Eugene Karpenko @ CaptainGPU
// https://twitter.com/CaptainGPU
//

#pragma once

#include "renderer.hxx"
#include "uniform.hxx"

class FrameBuffer;

enum SceneRendererPasses
{
    BASE_PASS,
    BOUND_PASS,
    POSTPROCESSING_PASS,
    
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
    void renderPostProcessingPass(RenderInfo& renderInfo);
    
private:
    bool m_renderBasePass;
    bool m_renderBoundPass;
    bool m_renderPostProcessing;

    glm::vec3 m_boundColor;

    Uniform* m_matrixModelUniform;
    Uniform* m_matrixViewUniform;
    Uniform* m_matrixProjectionUniform;
    Uniform* m_timeUniform;
    Uniform* m_basePassGammaUniform;

    Uniform* m_boundMatrixModelUniform;
    Uniform* m_boundMatrixViewUniform;
    Uniform* m_boundMatrixProjectionUniform;
    Uniform* m_boundColorUniform;
    
    Uniform* m_sceneTextureUniform;
    Uniform* m_chromaticAberrationUniform;
    Uniform* m_sepiaUniform;
    Uniform* m_filmGrainUniform;
    Uniform* m_postProcessTimeUniform;
    Uniform* m_vignetteUniform;
    Uniform* m_postProcessGammaUniform;

    Uniform* m_basePassAlbedoUniform;
    Uniform* m_baseLightColorUniform;
    Uniform* m_basePassAmbientColorUniform;
    Uniform* m_basePasSmoothnessUniform;
    Uniform* m_basePassAmbientStrengthUniform;
    Uniform* m_basePassSpecularStrengthUniform;
    Uniform* m_basePassCameraPosition;

    glm::vec3 m_basePassAlbedo;
    glm::vec3 m_basePassLightColor;
    glm::vec3 m_basePassAmbientColor;
    float m_basePassSmoothness;
    float m_basePassAmbientStrength;
    float m_basePassSpecularStrength;
    glm::vec3 m_sceneColor;

    
    float m_chAberrPower;
    float m_sepia;
    float m_filmGrain;
    float m_vignette;
    float m_gamma;
    
    FrameBuffer* m_frameBuffer;
private:
};
