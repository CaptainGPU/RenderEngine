//
// Created by Eugene Karpenko @ CaptainGPU
// https://twitter.com/CaptainGPU
//

#pragma once

#include "renderer.hxx"
#include "uniform.hxx"

#define MAX_POINT_LIGHTS 4
#define MAX_SPOT_LIGHTS 4

class FrameBuffer;
class Mesh;
struct PointLightData;
struct SpotLightData;
struct SunLightData;

enum SceneRendererPasses
{
    SUNLIGHT_SHADOW_PASS,
    BASE_PASS,
    BOUND_PASS,
    LIGHT_OBJECTS_PASS,
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
    void renderSunLightShadowPass(SunLightData& sunLightData, RenderInfo& renderInfo);
    void renderBasePass(std::vector<PointLightData>& lights, std::vector<SpotLightData>& spots, SunLightData& sunLightData, RenderInfo& renderInfo);
    void renderBoundPass(RenderInfo& renderInfo);
    void renderLightObjectsPass(std::vector<PointLightData>& lights, std::vector<SpotLightData>& spots, SunLightData& sunLightData, RenderInfo& renderInfo);
    void renderPostProcessingPass(RenderInfo& renderInfo);
    
    void constructLightsData(std::vector<PointLightData>& lights, std::vector<SpotLightData>& spots, SunLightData& sunLightData);
    
private:
    bool m_renderBasePass;
    bool m_renderBoundPass;
    bool m_renderLightObjectsPass;
    bool m_renderPostProcessing;
    bool m_renderSunLighShadowMap;
    
    bool m_renderPointLights;
    bool m_renderSpotLights;

    glm::vec3 m_boundColor;

    Uniform* m_sunLightShadowPassModelUniform;
    Uniform* m_sunLightShadowPassViewUniform;
    Uniform* m_sunLightShadowPassProjectionUniform;

    glm::mat4 m_sunLightProjectionMatrix;

    Uniform* m_matrixModelUniform;
    Uniform* m_matrixViewUniform;
    Uniform* m_matrixProjectionUniform;
    Uniform* m_timeUniform;
    Uniform* m_basePassGammaUniform;

    Uniform* m_boundMatrixModelUniform;
    Uniform* m_boundMatrixViewUniform;
    Uniform* m_boundMatrixProjectionUniform;
    Uniform* m_boundColorUniform;
    
    Uniform* m_lightObjectMatrixModelUniform;
    Uniform* m_lightObjectMatrixViewUniform;
    Uniform* m_lightObjectMatrixProjectionUniform;
    Uniform* m_lightObjectColorUniform;
    Uniform* m_lightObjectGammaUniform;
    
    Uniform* m_sceneTextureUniform;
    Uniform* m_sunLightShadowTextureUniform;
    Uniform* m_chromaticAberrationUniform;
    Uniform* m_sepiaUniform;
    Uniform* m_filmGrainUniform;
    Uniform* m_postProcessTimeUniform;
    Uniform* m_vignetteUniform;
    Uniform* m_postProcessGammaUniform;
    Uniform* m_postProcessShowShadowMapUniform;

    Uniform* m_basePassAlbedoUniform;
    Uniform* m_baseLightColorUniform;
    Uniform* m_basePassSunDirectionUniform;
    Uniform* m_basePassSunIntensityUniform;
    Uniform* m_basePassAmbientColorUniform;
    Uniform* m_basePasSmoothnessUniform;
    Uniform* m_basePassAmbientStrengthUniform;
    Uniform* m_basePassSpecularStrengthUniform;
    Uniform* m_basePassCameraPosition;
    Uniform* m_basePassSunSpaceMatrixUniform;
    Uniform* m_basePassSunShadowTextureUniform;

    glm::vec3 m_basePassAlbedo;
    glm::vec3 m_basePassLightColor;
    glm::vec3 m_basePassAmbientColor;
    float m_basePassSmoothness;
    float m_basePassAmbientStrength;
    float m_basePassSpecularStrength;
    Uniform* m_basePassPointLightColor[MAX_POINT_LIGHTS];
    Uniform* m_basePassPointLightsPosition[MAX_POINT_LIGHTS];
    Uniform* m_basePassPointLightsCount;
    Uniform* m_basePassSpotLightsCount;
    Uniform* m_basePassSpotLightsDirection[MAX_POINT_LIGHTS];
    Uniform* m_basePassSpotLightsPosition[MAX_POINT_LIGHTS];
    Uniform* m_basePassSpotLightsColor[MAX_POINT_LIGHTS];
    Uniform* m_basePassSpotLightsLinear[MAX_POINT_LIGHTS];
    Uniform* m_basePassSpotLightsConstant[MAX_POINT_LIGHTS];
    Uniform* m_basePassSpotLightsQuadratic[MAX_POINT_LIGHTS];
    Uniform* m_basePassSpotLightsInnerCut[MAX_POINT_LIGHTS];
    Uniform* m_basePassSpotLightsOutCut[MAX_POINT_LIGHTS];
    glm::vec3 m_sceneColor;

    
    float m_chAberrPower;
    float m_sepia;
    float m_filmGrain;
    float m_vignette;
    float m_gamma;
    
    FrameBuffer* m_frameBuffer;
    FrameBuffer* m_sunLightShadowFrameBuffer;
    
    Mesh* m_lightObjectMesh;
    Mesh* m_spotLightMesh;
    Mesh* m_sunLightMesh;

    unsigned int m_frameWidth;
    unsigned int m_frameHeight;

    unsigned int m_depthMapWidht;
    unsigned int m_depthMapHeight;
};
