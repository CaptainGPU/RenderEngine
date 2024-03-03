//
// Created by Eugene Karpenko @ CaptainGPU
// https://twitter.com/CaptainGPU
//
#include "sceneRenderer.hxx"
#include "engine.hxx"
#include "render.hxx"
#include "imgui.h"
#include "screenRenderPass.hxx"
#include "pointLightGameObject.hxx"
#include "spotLight.hxx"
#include "meshLoader.hxx"
#include "sunLight.hxx"
#include "math.hxx"
#include "spotLightShadow.hxx"
#include "pointLightShadow.hxx"

#include "forwardBasePass.hxx"
#include "mobileSSAOPass.hxx"

#include <glm/gtc/type_ptr.hpp>

SceneRenderer::SceneRenderer()
:Renderer("SceneRenderer"),
m_matrixModelUniform(nullptr),
m_matrixViewUniform(nullptr),
m_matrixProjectionUniform(nullptr),
m_timeUniform(nullptr),
m_boundMatrixModelUniform(nullptr),
m_boundMatrixViewUniform(nullptr),
m_boundMatrixProjectionUniform(nullptr),
m_boundColorUniform(nullptr),
m_sceneTextureUniform(nullptr),
m_renderPostProcessing(true),
m_boundColor(1.0),
m_frameBuffer(nullptr),
m_chromaticAberrationUniform(nullptr),
m_chAberrPower(0.0),
m_sepiaUniform(nullptr),
m_sepia(0.0),
m_filmGrainUniform(nullptr),
m_filmGrain(0.0),
m_vignetteUniform(nullptr),
m_vignette(0.0),
m_postProcessGammaUniform(nullptr),
m_gamma(2.2),
m_basePassGammaUniform(nullptr),
m_basePassMaterialBaseColorUniform(nullptr),
m_basePassAlbedo(glm::vec3(1.0)),
m_baseLightColorUniform(nullptr),
m_basePassLightColor(glm::vec3(1.0)),
m_basePassAmbientColorUniform(nullptr),
m_basePassAmbientColor(glm::vec3(1.0)),
m_basePasSmoothnessUniform(nullptr),
m_basePassSmoothness(0.214),
m_basePassAmbientStrengthUniform(nullptr),
m_basePassAmbientStrength(0.006),
m_basePassSpecularStrengthUniform(nullptr),
m_basePassSpecularStrength(0.5),
m_basePassCameraPosition(nullptr),
m_sceneColor(glm::vec3(.0))
{
    m_renderPointLights = false;
    m_renderSpotLights = false;
    m_renderSunLighShadowMap = false;

    m_renderMobileSSAOPass = true;
    m_renderBoundPass = false;
    m_renderBasePass = false;
    m_renderLightObjectsPass = false;

    // SunLight shadow Pass

    m_sunLightShadowFrameBuffer = nullptr;

    m_sunLightShadowPassModelUniform = nullptr;
    m_sunLightShadowPassViewUniform = nullptr;
    m_sunLightShadowPassProjectionUniform = nullptr;

    float near_plane = 1.0f, far_plane = 100.0f;
    m_sunLightProjectionMatrix = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
    
    // Base PASS
    
    m_basePassSunDirectionUniform = nullptr;
    m_basePassSunIntensityUniform = nullptr;
    m_basePassSunSpaceMatrixUniform = nullptr;
    m_basePassSunShadowTextureUniform = nullptr;
    m_basePassSpotLightsShadowCountUniform = nullptr;
    m_basePassPointLightsShadowCountUniform = nullptr;
    
    for (size_t i = 0; i < MAX_POINT_LIGHTS; i++)
    {   
        m_basePassPointLightColor[i] = nullptr;
        m_basePassPointLightsPosition[i] = nullptr;
        m_basePassPointLightsRadiusUniform[i] = nullptr;
    }
    
    for (size_t i = 0; i < MAX_SPOT_LIGHTS; i++)
    {
        m_basePassSpotLightsPosition[i] = nullptr;
        m_basePassSpotLightsDirection[i] = nullptr;
        m_basePassSpotLightsColor[i] = nullptr;
        m_basePassSpotLightsInvRange[i] = nullptr;
        m_basePassSpotLightsShadowMapVP[i] = nullptr;
        m_basePassSpotLightsShadowMapTextureUniform[i] = nullptr;
        m_basePassSpotlightsOutCutOff[i] = nullptr;
        m_basePassSpotlightsCutOff[i] = nullptr;
    }
    
    m_basePassPointLightsCount = nullptr;
    m_basePassSpotLightsCount = nullptr;
    m_basePassShadowDistanceUniform = nullptr;
    
    // Light objects PASS
    
    m_lightObjectGammaUniform = nullptr;
    
    m_lightObjectMesh = nullptr;
    m_spotLightMesh = nullptr;
    m_sunLightMesh = nullptr;
    
    m_lightObjectMatrixModelUniform = nullptr;
    m_lightObjectMatrixViewUniform = nullptr;
    m_lightObjectMatrixProjectionUniform = nullptr;
    m_lightObjectColorUniform = nullptr;

    // Post-processing pass

    m_sunLightShadowTextureUniform = nullptr;
    m_postProcessShowShadowMapUniform = nullptr;

    // Data

    Engine::get()->getWindowBufferSize(m_frameWidth, m_frameHeight);
    m_depthMapWidht = 1024;
    m_depthMapHeight = 1024;
    
    m_pointLightDynamicShadowCount = MAX_POINT_LIGHTS;
    m_spotLightDynamicShadowCount = MAX_SPOT_LIGHTS;
}

void SceneRenderer::init()
{
    Renderer::init();
    m_renderPasses.resize(SceneRendererPasses::PASS_COUNT);

    for (size_t i = 0; i < SceneRendererPasses::PASS_COUNT; i++)
    {
        RenderPass* renderPass = nullptr;

        SceneRendererPasses pass = static_cast<SceneRendererPasses>(i);

        if (pass == SUNLIGHT_SHADOW_PASS)
        {
            std::vector<std::string> uniformNames = { "u_modelMatrix", "u_viewMatrix", "u_projectionMatrix" };

            renderPass = new RenderPass();
            renderPass->makeProgram("sunLightShadow", "sunLightShadow");
            renderPass->registerUniforms(uniformNames);

            m_sunLightShadowPassModelUniform = renderPass->getUniform(uniformNames[0]);
            m_sunLightShadowPassViewUniform = renderPass->getUniform(uniformNames[1]);
            m_sunLightShadowPassProjectionUniform = renderPass->getUniform(uniformNames[2]);
        }

        if (pass == SPOTLIGHT_SHADOW_PASS)
        {
            renderPass = registerSpotLightShadowPass();
        }

        if (pass == POINTLIGHT_SHADOW_PASS)
        {
            renderPass = registerPointLightShadowPass();
        }

        if (pass == MOBILE_SSAO_PASS)
        {
            renderPass = registerMobileSSAOORenderPass();
        }

        if (pass == BASE_PASS)
        {
            std::vector<std::string> uniformNames = {
                "u_modelMatrix",                        // 0
                "u_viewMatrix",                         // 1
                "u_projectionMatrix", 
                "time", 
                "u_gamma", 
                "u_materialBaseColor", 
                "u_lightColor", 
                "u_ambientColor", 
                "u_smoothness", 
                "u_ambientStrength", 
                "u_specularStrength", 
                "u_cameraPosition", 
                "u_pointLightsCount", 
                "u_spotLightsCount", 
                "u_sunDirection", 
                "u_sunItensity", 
                "u_sunLightSpaceMatrix", 
                "u_texture_0", 
                "u_shadowDistance",
                "u_texture_1", 
                "u_texture_2", 
                "u_texture_3", 
                "u_texture_4", 
                "u_texture_5", 
                "u_texture_6",
                "u_texture_7",
                "u_texture_8",
                "u_spotLightShadowCount",
                "u_pointLightShadowCount"
            };
            
            for (size_t i = 0; i < MAX_POINT_LIGHTS; i++)
            {
                char locBuff[100] = { '\0' };
                
                snprintf(locBuff, sizeof(locBuff), "u_pointLights[%zu].color", i);
                std::string name = std::string(locBuff);
                uniformNames.push_back(name);
                
                snprintf(locBuff, sizeof(locBuff), "u_pointLights[%zu].position", i);
                name = std::string(locBuff);
                uniformNames.push_back(name);

                snprintf(locBuff, sizeof(locBuff), "u_pointLights[%zu].radius", i);
                name = std::string(locBuff);
                uniformNames.push_back(name);
            }
            
            for (size_t i = 0; i < MAX_SPOT_LIGHTS; i++)
            {
                char locBuff[100] = { '\0' };
                
                snprintf(locBuff, sizeof(locBuff), "u_spotLights[%zu].direction", i);
                std::string name = std::string(locBuff);
                uniformNames.push_back(name);
                
                snprintf(locBuff, sizeof(locBuff), "u_spotLights[%zu].position", i);
                name = std::string(locBuff);
                uniformNames.push_back(name);
                
                snprintf(locBuff, sizeof(locBuff), "u_spotLights[%zu].color", i);
                name = std::string(locBuff);
                uniformNames.push_back(name);

                snprintf(locBuff, sizeof(locBuff), "u_spotLights[%zu].invRange", i);
                name = std::string(locBuff);
                uniformNames.push_back(name);
                
                snprintf(locBuff, sizeof(locBuff), "u_spotLights[%zu].cutOff", i);
                name = std::string(locBuff);
                uniformNames.push_back(name);
                
                snprintf(locBuff, sizeof(locBuff), "u_spotLights[%zu].outerCutOff", i);
                name = std::string(locBuff);
                uniformNames.push_back(name);

                snprintf(locBuff, sizeof(locBuff), "u_spotLightsVPMatrix[%zu]", i);
                name = std::string(locBuff);
                uniformNames.push_back(name);
            }
            
            renderPass = new RenderPass();
            renderPass->makeProgram("mesh", "mesh");
            renderPass->registerUniforms(uniformNames);
            
            m_matrixModelUniform = renderPass->getUniform(uniformNames[0]);
            m_matrixViewUniform = renderPass->getUniform(uniformNames[1]);
            m_matrixProjectionUniform = renderPass->getUniform(uniformNames[2]);
            m_timeUniform = renderPass->getUniform(uniformNames[3]);
            m_basePassGammaUniform = renderPass->getUniform(uniformNames[4]);
            m_basePassMaterialBaseColorUniform = renderPass->getUniform(uniformNames[5]);
            m_baseLightColorUniform = renderPass->getUniform(uniformNames[6]);
            m_basePassAmbientColorUniform = renderPass->getUniform(uniformNames[7]);
            m_basePasSmoothnessUniform = renderPass->getUniform(uniformNames[8]);
            m_basePassAmbientStrengthUniform = renderPass->getUniform(uniformNames[9]);
            m_basePassSpecularStrengthUniform = renderPass->getUniform(uniformNames[10]);
            m_basePassCameraPosition = renderPass->getUniform(uniformNames[11]);
            m_basePassPointLightsCount = renderPass->getUniform(uniformNames[12]);
            m_basePassSpotLightsCount = renderPass->getUniform(uniformNames[13]);
            m_basePassSunDirectionUniform = renderPass->getUniform(uniformNames[14]);
            m_basePassSunIntensityUniform = renderPass->getUniform(uniformNames[15]);
            m_basePassSunSpaceMatrixUniform = renderPass->getUniform(uniformNames[16]);
            m_basePassSunShadowTextureUniform = renderPass->getUniform(uniformNames[17]);
            m_basePassShadowDistanceUniform = renderPass->getUniform(uniformNames[18]);
            m_basePassSpotLightsShadowMapTextureUniform[0] = renderPass->getUniform(uniformNames[19]);
            m_basePassSpotLightsShadowMapTextureUniform[1] = renderPass->getUniform(uniformNames[20]);
            m_basePassSpotLightsShadowMapTextureUniform[2] = renderPass->getUniform(uniformNames[21]);
            m_basePassSpotLightsShadowMapTextureUniform[3] = renderPass->getUniform(uniformNames[22]);
            m_basePassPointLightCubeTexture[0] = renderPass->getUniform(uniformNames[23]);
            m_basePassPointLightCubeTexture[1] = renderPass->getUniform(uniformNames[24]);
            m_basePassPointLightCubeTexture[2] = renderPass->getUniform(uniformNames[25]);
            m_basePassPointLightCubeTexture[3] = renderPass->getUniform(uniformNames[26]);
            m_basePassSpotLightsShadowCountUniform = renderPass->getUniform(uniformNames[27]);
            m_basePassPointLightsShadowCountUniform = renderPass->getUniform(uniformNames[28]);

            
            for (size_t i = 0; i < MAX_POINT_LIGHTS; i++)
            {
                char locBuff[100] = { '\0' };
                
                snprintf(locBuff, sizeof(locBuff), "u_pointLights[%zu].color", i);
                std::string name = std::string(locBuff);
                m_basePassPointLightColor[i] = renderPass->getUniform(name);
                
                snprintf(locBuff, sizeof(locBuff), "u_pointLights[%zu].position", i);
                name = std::string(locBuff);
                m_basePassPointLightsPosition[i] = renderPass->getUniform(name);

                snprintf(locBuff, sizeof(locBuff), "u_pointLights[%zu].radius", i);
                name = std::string(locBuff);
                m_basePassPointLightsRadiusUniform[i] = renderPass->getUniform(name);
            }
            
            for (size_t i = 0; i < MAX_SPOT_LIGHTS; i++)
            {
                char locBuff[100] = { '\0' };
                
                snprintf(locBuff, sizeof(locBuff), "u_spotLights[%zu].direction", i);
                std::string name = std::string(locBuff);
                m_basePassSpotLightsDirection[i] = renderPass->getUniform(name);
                
                snprintf(locBuff, sizeof(locBuff), "u_spotLights[%zu].position", i);
                name = std::string(locBuff);
                m_basePassSpotLightsPosition[i] = renderPass->getUniform(name);
                
                snprintf(locBuff, sizeof(locBuff), "u_spotLights[%zu].color", i);
                name = std::string(locBuff);
                m_basePassSpotLightsColor[i] = renderPass->getUniform(name);

                snprintf(locBuff, sizeof(locBuff), "u_spotLights[%zu].invRange", i);
                name = std::string(locBuff);
                m_basePassSpotLightsInvRange[i] = renderPass->getUniform(name);
                
                snprintf(locBuff, sizeof(locBuff), "u_spotLights[%zu].cutOff", i);
                name = std::string(locBuff);
                m_basePassSpotlightsCutOff[i] = renderPass->getUniform(name);
                
                snprintf(locBuff, sizeof(locBuff), "u_spotLights[%zu].outerCutOff", i);
                name = std::string(locBuff);
                m_basePassSpotlightsOutCutOff[i] = renderPass->getUniform(name);

                snprintf(locBuff, sizeof(locBuff), "u_spotLightsVPMatrix[%zu]", i);
                name = std::string(locBuff);
                m_basePassSpotLightsShadowMapVP[i] = renderPass->getUniform(name);
            }
        }

        if (pass == FORWARD_BASE_PASS)
        {
            renderPass = registerForwardBasePass();
        }

        if (pass == BOUND_PASS)
        {
            std::vector<std::string> uniformNames = { "u_modelMatrix", "u_viewMatrix", "u_projectionMatrix", "u_boundColor"};

            renderPass = new RenderPass();
            renderPass->makeProgram("meshBound", "meshBound");
            renderPass->registerUniforms(uniformNames);

            m_boundMatrixModelUniform = renderPass->getUniform(uniformNames[0]);
            m_boundMatrixViewUniform = renderPass->getUniform(uniformNames[1]);
            m_boundMatrixProjectionUniform = renderPass->getUniform(uniformNames[2]);
            m_boundColorUniform = renderPass->getUniform(uniformNames[3]);
        }
        
        if (pass == LIGHT_OBJECTS_PASS)
        {
            std::vector<std::string> uniformNames = { "u_modelMatrix", "u_viewMatrix", "u_projectionMatrix", "u_color", "u_gamma" };
            
            renderPass = new RenderPass();
            renderPass->makeProgram("light", "light");
            renderPass->registerUniforms(uniformNames);
            
            m_lightObjectMatrixModelUniform = renderPass->getUniform(uniformNames[0]);
            m_lightObjectMatrixViewUniform = renderPass->getUniform(uniformNames[1]);
            m_lightObjectMatrixProjectionUniform = renderPass->getUniform(uniformNames[2]);
            m_lightObjectColorUniform = renderPass->getUniform(uniformNames[3]);
            m_lightObjectGammaUniform = renderPass->getUniform(uniformNames[4]);
        }
        
        if (pass == POSTPROCESSING_PASS)
        {
            std::vector<std::string> uniformNames = { "u_texture_0", "u_chromaticAberration", "u_sepia", "u_filmGrain", "u_vignette", "u_time", "u_gamma", "u_texture_1", "u_showShadowMap"};
            
            renderPass = new ScreenRenderPass();
            renderPass->makeProgram("fullScreen", "fullScreen");
            renderPass->registerUniforms(uniformNames);
            
            m_sceneTextureUniform = renderPass->getUniform(uniformNames[0]);
            m_chromaticAberrationUniform = renderPass->getUniform(uniformNames[1]);
            m_sepiaUniform = renderPass->getUniform(uniformNames[2]);
            m_filmGrainUniform = renderPass->getUniform(uniformNames[3]);
            m_vignetteUniform = renderPass->getUniform(uniformNames[4]);
            m_postProcessTimeUniform = renderPass->getUniform(uniformNames[5]);
            m_postProcessGammaUniform = renderPass->getUniform(uniformNames[6]);
            m_sunLightShadowTextureUniform = renderPass->getUniform(uniformNames[7]);
            m_postProcessShowShadowMapUniform = renderPass->getUniform(uniformNames[8]);
        }

        m_renderPasses[i] = renderPass;
    }
    
    m_frameBuffer = Render::createFrameBuffer();
    m_sunLightShadowFrameBuffer = Render::createDepthMapFrameBuffer(1024, 1024);

    m_lightObjectMesh = loadMesh("light.mesh");
    m_spotLightMesh = loadMesh("spot.mesh");
    m_sunLightMesh = loadMesh("sun.mesh");

    initMobileSSAOPassData();
}

void SceneRenderer::finish()
{
    Renderer::finish();
}

/*std::vector<glm::vec4> getFrustumCornersWorldSpace(const glm::mat4& proj, const glm::mat4& view)
{
    const glm::mat4 inv = glm::inverse(view);

    std::vector<glm::vec4> frustumCorners;

    for (unsigned int x = 0; x < 2; ++x)
    {
        for (unsigned int y = 0; y < 2; ++y)
        {
            for (unsigned int z = 0; z < 2; ++z)
            {
                const glm::vec4 pt = inv * glm::vec4(
                    2.0f * x - 1.0f,
                    2.0f * y - 1.0f,
                    2.0f * z - 1.0f,
                    1.0f
                );
                frustumCorners.push_back(pt / pt.w);
            }
        }
    }

    return frustumCorners;
}*/

void SceneRenderer::renderSunLightShadowPass(SunLightData& sunLightData, RenderInfo& renderInfo)
{
    SceneManager* manager = Engine::get()->getSceneManager();
    Scene* scene = manager->getScene();
    Camera* camera = scene->getCamera();

    size_t numGameObject = scene->getGameObjectCount();

    RenderPass* renderPass = m_renderPasses[SceneRendererPasses::SUNLIGHT_SHADOW_PASS];
    Render::startRenderPass(renderPass, renderInfo);

    glm::vec3 lightDir = sunLightData.direction;
    glm::vec3 lightWorldPos = glm::vec3(0.0f, 0.0, 10.0);
    //lightDir = glm::vec3(0.0f, -1.0f, 0.0f);

    glm::mat4 viewMatrix = glm::lookAt(lightWorldPos, lightWorldPos + lightDir, glm::vec3(0.0f, 1.0f, 0.0f));
    //m_sunLightShadowPassViewUniform->setMatrix4x4(viewMatrix);

    glm::mat4 projection_matrix = glm::ortho(-10.0, 10.0, -5.0, 5.0, -5.0, 5.0);
    //m_sunLightShadowPassProjectionUniform->setMatrix4x4(projection_matrix);

    sunLightData.view = viewMatrix;
    m_sunLightProjectionMatrix = projection_matrix * viewMatrix;
    m_sunLightProjectionMatrix = calculateLightProjectionViewMatrix(lightDir, camera);
    m_sunLightShadowPassProjectionUniform->setMatrix4x4(m_sunLightProjectionMatrix);

    for (size_t i = 0; i < numGameObject; i++)
    {
        GameObject* gameObject = scene->getGameObject(i);
        unsigned int meshCount = gameObject->getMeshCount();

        if (meshCount == 0 || !gameObject->isRenderingObject())
        {
            continue;
        }

        glm::mat4 modelMatrix = gameObject->getModelMatrix();
        m_sunLightShadowPassModelUniform->setMatrix4x4(modelMatrix);

        for (size_t j = 0; j < meshCount; j++)
        {
            Mesh* mesh = gameObject->getMesh(j);

            Render::drawMesh(mesh, renderInfo);
        }
    }


    Render::endRenderPass(renderPass);
}

void SceneRenderer::renderBasePass(std::vector<PointLightData>& lights, std::vector<SpotLightData>& spots, SunLightData& sunLightData, RenderInfo& renderInfo)
{
    SceneManager* manager = Engine::get()->getSceneManager();
    Scene* scene = manager->getScene();
    Camera* camera = scene->getCamera();

    size_t numGameObject = scene->getGameObjectCount();

    RenderPass* renderPass = m_renderPasses[SceneRendererPasses::BASE_PASS];

    Render::startRenderPass(renderPass, renderInfo);

    glm::mat4& viewMatrix = camera->getViewMatrix();
    m_matrixViewUniform->setMatrix4x4(viewMatrix);

    glm::mat4& projection_matrix = scene->getCamera()->getProjectionMatrix();
    m_matrixProjectionUniform->setMatrix4x4(projection_matrix);

    float time = Engine::get()->getGameTime();
    m_timeUniform->setFloat(time);

    glm::vec3 cameraPosition = camera->getPosition();
    
    m_basePassSunDirectionUniform->setVec3(sunLightData.direction);
    m_baseLightColorUniform->setVec3(sunLightData.color);
    m_basePassSunIntensityUniform->setFloat(sunLightData.intensity);

    m_basePassGammaUniform->setFloat(m_gamma);
  
    m_basePassAmbientColorUniform->setVec3(m_basePassAmbientColor);
    m_basePasSmoothnessUniform->setFloat(m_basePassSmoothness);
    m_basePassAmbientStrengthUniform->setFloat(m_basePassAmbientStrength);
    m_basePassSpecularStrengthUniform->setFloat(m_basePassSpecularStrength);
    m_basePassCameraPosition->setVec3(cameraPosition);
    
    float shadowDistance = camera->getShadowDistance();
    m_basePassShadowDistanceUniform->setFloat(shadowDistance);

    glm::mat4 sunSpaceMatrix = m_sunLightProjectionMatrix;// *sunLightData.view;
    m_basePassSunSpaceMatrixUniform->setMatrix4x4(sunSpaceMatrix);

    Texture* texture = m_sunLightShadowFrameBuffer->getDepthTexture();
    //Texture* texture = getSpotLightShadowMapTexture();
    m_basePassSunShadowTextureUniform->setTexture(texture, 0);
    
    int lightsCount = lights.size();
    int spotLightCount = spots.size();
    
    if (!m_renderSpotLights)
    {
        spotLightCount = 0;
    }
    
    if (!m_renderPointLights)
    {
        lightsCount = 0;
    }
    
    m_basePassPointLightsCount->setInt(lightsCount);
    m_basePassSpotLightsCount->setInt(spotLightCount);
    m_basePassSpotLightsShadowCountUniform->setInt(m_spotLightDynamicShadowCount);
    m_basePassPointLightsShadowCountUniform->setInt(m_pointLightDynamicShadowCount);
    
    glm::vec3 lColor = glm::vec3(1.0, .0, .0);

    std::vector<Texture*> pointLightShadowMaps = getPointLightShadowMapTexture();
    
    for (size_t i = 0; i < lightsCount; i++)
    {
        PointLightData data = lights[i];
        m_basePassPointLightColor[i]->setVec3(data.color);
        m_basePassPointLightsPosition[i]->setVec3(data.position);
        m_basePassPointLightsRadiusUniform[i]->setFloat(data.radius);

        Texture* texture = pointLightShadowMaps[i];
        m_basePassPointLightCubeTexture[i]->setCubeTexture(texture, 5 + i);
    }

    
    std::vector<Texture*> spotLightShadowMaps = getSpotLightShadowMapTexture();
    for (size_t i = 0; i < spotLightCount; i++)
    {
        SpotLightData data = spots[i];
        m_basePassSpotLightsDirection[i]->setVec3(data.direction);
        m_basePassSpotLightsPosition[i]->setVec3(data.position);
        m_basePassSpotLightsColor[i]->setVec3(data.color);

        m_basePassSpotLightsInvRange[i]->setFloat(data.invRange);

        m_basePassSpotLightsShadowMapVP[i]->setMatrix4x4(data.vpMatrix);
        
        m_basePassSpotlightsCutOff[i]->setFloat(data.innerCut);
        m_basePassSpotlightsOutCutOff[i]->setFloat(data.outCut);

        Texture* texture = spotLightShadowMaps[i];
        m_basePassSpotLightsShadowMapTextureUniform[i]->setTexture(texture, 1 + i);
    }

    for (size_t i = 0; i < numGameObject; i++)
    {
        GameObject* gameObject = scene->getGameObject(i);
        unsigned int meshCount = gameObject->getMeshCount();

        if (meshCount == 0 || !gameObject->isRenderingObject())
        {
            continue;
        }

        glm::mat4 modelMatrix = gameObject->getModelMatrix();
        m_matrixModelUniform->setMatrix4x4(modelMatrix);

        for (size_t j = 0; j < meshCount; j++)
        {
            Mesh* mesh = gameObject->getMesh(j);
            Material* material = mesh->getMaterial();
            m_basePassMaterialBaseColorUniform->setVec3(material->color);

            Render::drawMesh(mesh, renderInfo);
        }
    }

    Render::endRenderPass(renderPass);
}

void SceneRenderer::renderBoundPass(RenderInfo& renderInfo)
{
    SceneManager* manager = Engine::get()->getSceneManager();
    Scene* scene = manager->getScene();
    Camera* camera = scene->getCamera();

    size_t numGameObject = scene->getGameObjectCount();

    RenderPass* renderPass = m_renderPasses[SceneRendererPasses::BOUND_PASS];

    Render::startRenderPass(renderPass, renderInfo);

    glm::mat4& viewMatrix = camera->getViewMatrix();
    m_boundMatrixViewUniform->setMatrix4x4(viewMatrix);

    glm::mat4& projection_matrix = scene->getCamera()->getProjectionMatrix();
    m_boundMatrixProjectionUniform->setMatrix4x4(projection_matrix);

    m_boundColorUniform->setVec3(m_boundColor);

    std::vector<SpotLight*> spots;

    for (size_t i = 0; i < numGameObject; i++)
    {
        GameObject* gameObject = scene->getGameObject(i);
        
        unsigned int meshCount = gameObject->getMeshCount();

        SpotLight* spot = dynamic_cast<SpotLight*>(gameObject);
        if (spot)
        {
            spots.push_back(spot);
        }

        if (meshCount == 0 || !gameObject->isRenderingObject())
        {
            continue;
        }

        glm::mat4 modelMatrix = gameObject->getModelMatrix();
        m_boundMatrixModelUniform->setMatrix4x4(modelMatrix);

        for (size_t j = 0; j < meshCount; j++)
        {
            Mesh* mesh = gameObject->getMesh(j);
            if (mesh)
            {
                Render::drawMeshBound(mesh->getMeshBound(), renderInfo);
            }
        }
    }

    if (false)
    {
        Camera* defaultCamera = scene->getDefaultCamera();
        if (defaultCamera)
        {
            glm::mat4 modelMatrix = glm::mat4(1.0);
            m_boundMatrixModelUniform->setMatrix4x4(modelMatrix);
            defaultCamera->renderBounds(m_boundColorUniform, renderInfo);
        }
    }

    if (m_renderSpotLights)
    {
        for (SpotLight* spot : spots)
        {
            MeshBound* bound = spot->getMeshBound();

            if (spot && bound)
            {
                glm::vec3 spotBoundColor = spot->getColor();
                m_boundColorUniform->setVec3(spotBoundColor);

                glm::mat4 modelMatrix = spot->getModelMatrix();
                m_boundMatrixModelUniform->setMatrix4x4(modelMatrix);



                Render::drawMeshBound(bound, renderInfo);
            }
        }
    }

    Render::endRenderPass(renderPass);
}

void SceneRenderer::renderLightObjectsPass(std::vector<PointLightData>& lights, std::vector<SpotLightData>& spots, SunLightData& sunLightData, RenderInfo& renderInfo)
{
    SceneManager* manager = Engine::get()->getSceneManager();
    Scene* scene = manager->getScene();
    Camera* camera = scene->getCamera();
    
    RenderPass* renderPass = m_renderPasses[SceneRendererPasses::LIGHT_OBJECTS_PASS];
    
    Render::startRenderPass(renderPass, renderInfo);
    
    glm::mat4& viewMatrix = camera->getViewMatrix();
    m_lightObjectMatrixViewUniform->setMatrix4x4(viewMatrix);

    glm::mat4& projection_matrix = camera->getProjectionMatrix();
    m_lightObjectMatrixProjectionUniform->setMatrix4x4(projection_matrix);
    
    m_lightObjectGammaUniform->setFloat(m_gamma);
    
    size_t lightsCount = lights.size();
    size_t spotLightCount = spots.size();
    
    if (!m_renderSpotLights)
    {
        spotLightCount = 0;
    }
    
    if (!m_renderPointLights)
    {
        lightsCount = 0;
    }
    
    for (size_t i = 0; i < lightsCount; i++)
    {
        PointLightData& data = lights[i];
        m_lightObjectMatrixModelUniform->setMatrix4x4(data.model);
        m_lightObjectColorUniform->setVec3(data.color);
        
        Render::drawMesh(m_lightObjectMesh, renderInfo);
    }
    
    for (size_t i = 0; i < spotLightCount; i++)
    {
        SpotLightData& data = spots[i];
        m_lightObjectMatrixModelUniform->setMatrix4x4(data.model);
        m_lightObjectColorUniform->setVec3(data.color);
        
        Render::drawMesh(m_spotLightMesh, renderInfo);
    }
    
    m_lightObjectMatrixModelUniform->setMatrix4x4(sunLightData.model);
    m_lightObjectColorUniform->setVec3(sunLightData.color);
    Render::drawMesh(m_sunLightMesh, renderInfo);
    
    Render::endRenderPass(renderPass);
}

void SceneRenderer::renderPostProcessingPass(RenderInfo& renderInfo)
{
    Render::setViewport(0, 0, 800, 600);
    RenderPass* renderPass = m_renderPasses[SceneRendererPasses::POSTPROCESSING_PASS];
    ScreenRenderPass* pass = dynamic_cast<ScreenRenderPass*>(renderPass);

    Render::startRenderPass(renderPass, renderInfo);

    int showShadowMap = m_renderSunLighShadowMap ? 1 : 0;
    m_postProcessShowShadowMapUniform->setInt(showShadowMap);
    
    //Texture* texture = m_frameBuffer->getColorTexture();
    //Texture* texture = getForwardBasePassFrameBuffer()->getColorTexture();
    Texture* texture = getDepthPrePassFrameBuffer()->getColorTexture();
    //Texture* sunLightTexture = m_sunLightShadowFrameBuffer->getColorTexture();
    Texture* sunLightTexture = getSpotLightShadowMapColorTexture()[0];
    m_sceneTextureUniform->setTexture(texture, 0);
    m_sunLightShadowTextureUniform->setTexture(sunLightTexture, 1);
    
    float aberrationPower = 0.0;
    float sepia = .0;
    float filmGrain = .0;
    float vignette = .0;
    float gamma = 1.0;
    
    if (m_renderPostProcessing)
    {
        aberrationPower = m_chAberrPower;
        sepia = m_sepia;
        filmGrain = m_filmGrain;
        vignette = m_vignette;
        gamma = m_gamma;
    }
    
    m_chromaticAberrationUniform->setFloat(aberrationPower);
    m_sepiaUniform->setFloat(sepia);
    m_filmGrainUniform->setFloat(filmGrain);
    m_vignetteUniform->setFloat(vignette);
    m_postProcessGammaUniform->setFloat(gamma);
    
    float time = Engine::get()->getGameTime();
    m_postProcessTimeUniform->setFloat(time);
    
    pass->draw(renderInfo);
    
    Render::endRenderPass(renderPass);
}

void SceneRenderer::render(RenderInfo& renderInfo)
{
    std::vector<PointLightData> lights;
    std::vector<SpotLightData> spots;
    
    SunLightData sunLightData;
    
    constructLightsData(lights, spots, sunLightData);

    // Render SunLight Shadow Pass

    if (m_renderSunLighShadowMap)
    {

        Render::setViewport(0, 0, m_depthMapWidht, m_depthMapHeight);
        Render::useFrameBuffer(m_sunLightShadowFrameBuffer);

        Render::clearView(0.0, 0.0, 0.0, 1.0);

        renderSunLightShadowPass(sunLightData, renderInfo);

        Render::unUseFrameBuffer();
    }

    // Render SpotLight Shadow Pass

    if (m_renderSpotLights)
    {
        RenderPass* renderPass = m_renderPasses[SceneRendererPasses::SPOTLIGHT_SHADOW_PASS];
        renderSpotlightShadowsPass(renderPass, renderInfo, spots, m_spotLightDynamicShadowCount);
    }

    // Render PointLight Shadow Pass

    if (m_renderPointLights)
    {
        RenderPass* renderPass = m_renderPasses[SceneRendererPasses::POINTLIGHT_SHADOW_PASS];
        renderPointLightShadowsPass(renderPass, renderInfo, lights, m_pointLightDynamicShadowCount);
    }

    if (m_renderBasePass)
    {
        RenderPass* renderPass = m_renderPasses[SceneRendererPasses::FORWARD_BASE_PASS];
        renderForwardBasePass(m_frameWidth, m_frameHeight, renderPass, renderInfo);
    }

    if (m_renderMobileSSAOPass)
    {
        RenderPass* renderPass = m_renderPasses[SceneRendererPasses::MOBILE_SSAO_PASS];
        renderMobileSSAOPass(renderPass, renderInfo);
    }

    //// Render Base Pass
    //Render::setViewport(0, 0, m_frameWidth, m_frameHeight);
    //Render::useFrameBuffer(m_frameBuffer);

    //float gamma = m_gamma;

    //glm::vec3 sceneColor = glm::pow(m_sceneColor, glm::vec3(gamma));
    //
    //Render::clearView(sceneColor.r, sceneColor.g, sceneColor.b, 1.0);

    //if (m_renderBasePass)
    //{
    //    renderBasePass(lights, spots, sunLightData, renderInfo);
    //}

    //if (m_renderBoundPass)
    //{
    //    renderBoundPass(renderInfo);
    //}
    //
    //if (m_renderLightObjectsPass)
    //{
    //    renderLightObjectsPass(lights, spots, sunLightData, renderInfo);
    //}
    //
    //Render::unUseFrameBuffer();

    // Render PostProcess Pass
    
    Render::clearView(164.0f / 256.0f, 189.0f / 256.0f, 191.0f / 256.0f, 1.0);
    
    renderPostProcessingPass(renderInfo);
}

void SceneRenderer::drawDebugUI()
{  
    ImGui::Begin("Post-processing settings");
    
    ImGui::SliderFloat("Chromatic Aberrations", &m_chAberrPower, .0f, 4.0f);
    ImGui::SliderFloat("Sepia", &m_sepia, .0f, 1.0f);
    ImGui::SliderFloat("Film Grain", &m_filmGrain, .0f, 1.0f);
    ImGui::SliderFloat("Vignette", &m_vignette, .0f, 2.0f);
    ImGui::SliderFloat("Gamma", &m_gamma, 1.0f, 3.0f);
    
    ImGui::End();

    ImGui::Begin("Scene Renderer");
    ImGui::Checkbox("Base Pass", &m_renderBasePass);
    ImGui::Checkbox("Bound Pass", &m_renderBoundPass);
    ImGui::Checkbox("Show Lights", &m_renderLightObjectsPass);
    ImGui::Checkbox("Spot Lights Enable", &m_renderSpotLights);
    ImGui::Checkbox("Point Lights Enable", &m_renderPointLights);
    ImGui::Checkbox("Post-Processing Pass", &m_renderPostProcessing);
    ImGui::Checkbox("Show ShadowMap", &m_renderSunLighShadowMap);
    
    ImGui::SliderInt("PointLight Shadows", &m_pointLightDynamicShadowCount, 0, MAX_POINT_LIGHTS);
    ImGui::SliderInt("SpotLight Shadows", &m_spotLightDynamicShadowCount, 0, MAX_SPOT_LIGHTS);

    float* f = glm::value_ptr(m_boundColor);
    ImGui::ColorEdit3("bound", f);

    f = glm::value_ptr(m_sceneColor);
    ImGui::ColorEdit3("scene color", f);
    
    SceneManager* manager = Engine::get()->getSceneManager();
    Scene* scene = manager->getScene();
    SunLight* light = scene->getSunLight();
    
    if (light)
    {
        m_basePassLightColor = light->getColor();
        f = glm::value_ptr(m_basePassLightColor );
        ImGui::ColorEdit3("Sun color", f);
        light->setColor(m_basePassLightColor);
        
        float intensity = light->getIntensity();
        ImGui::SliderFloat("Sun intensity", &intensity, .0f, 1.0f);
        light->setIntensity(intensity);
    }
    

    ImGui::SliderFloat("Ambient Strength", &m_basePassAmbientStrength, .0f, 1.0f);
    ImGui::SliderFloat("Specular Strength", &m_basePassSpecularStrength, .0f, 1.0f);

    ImGui::SliderFloat("smoothness", &m_basePassSmoothness, .001f, 1.0f);

    if (false)
    {
        SunLight* sun = scene->getSunLight();
        float x = sun->getRotationX();
        float y = sun->getRotationY();
        float z = sun->getRotationZ();

        ImGui::SliderFloat("sunX", &x, 0.0f, 360.0f);
        ImGui::SliderFloat("sunY", &y, 0.0f, 360.0f);
        ImGui::SliderFloat("sunZ", &z, 0.0f, 360.0f);

        sun->SetRotateX(x);
        sun->SetRotateY(y);
        sun->SetRotateZ(z);
    }

    {
        Camera* camera = scene->getCamera();
        float shadowDistance = camera->getShadowDistance();
        ImGui::SliderFloat("Shadow Distance", &shadowDistance, 0.0f, 1.0f);
        camera->setShadowDistance(shadowDistance);
    }

    ImGui::End();
}

void SceneRenderer::constructLightsData(std::vector<PointLightData>& lights, std::vector<SpotLightData>& spots, SunLightData& sunLightData)
{
    SceneManager* manager = Engine::get()->getSceneManager();
    Scene* scene = manager->getScene();
    
    SunLight* sunLight = scene->getSunLight();
    sunLightData = SunLight::getSunLightData(sunLight);

    size_t numGameObject = scene->getGameObjectCount();
    
    for (size_t i = 0; i < numGameObject; i++)
    {
        GameObject* object = scene->getGameObject(i);
        if (!object)
        {
            continue;
        }
        
        PoinLightGameObject* pointLight = dynamic_cast<PoinLightGameObject*>(object);
        if (pointLight)
        {
            PointLightData data = pointLight->getData();
            lights.push_back(data);
        }
        
        SpotLight* spot = dynamic_cast<SpotLight*>(object);
        if (spot)
        {
            SpotLightData data = spot->getData();
            spots.push_back(data);
        }
    }
}
