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
m_renderBoundPass(false),
m_renderBasePass(true),
m_renderLightObjectsPass(true),
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
m_basePassAlbedoUniform(nullptr),
m_basePassAlbedo(glm::vec3(1.0)),
m_baseLightColorUniform(nullptr),
m_basePassLightColor(glm::vec3(1.0)),
m_basePassAmbientColorUniform(nullptr),
m_basePassAmbientColor(glm::vec3(1.0)),
m_basePasSmoothnessUniform(nullptr),
m_basePassSmoothness(0.214),
m_basePassAmbientStrengthUniform(nullptr),
m_basePassAmbientStrength(0.1),
m_basePassSpecularStrengthUniform(nullptr),
m_basePassSpecularStrength(0.5),
m_basePassCameraPosition(nullptr),
m_sceneColor(glm::vec3(.0))
{
    
    m_renderPointLights = true;
    m_renderSpotLights = true;
    
    // Base PASS
    for (size_t i = 0; i < MAX_POINT_LIGHTS; i++)
    {
        m_basePassPointLightColor[i] = nullptr;
        m_basePassPointLightsPosition[i] = nullptr;
    }
    
    for (size_t i = 0; i < MAX_SPOT_LIGHTS; i++)
    {
        m_basePassSpotLightsPosition[i] = nullptr;
        m_basePassSpotLightsDirection[i] = nullptr;
        m_basePassSpotLightsColor[i] = nullptr;
        m_basePassSpotLightsLinear[i] = nullptr;
        m_basePassSpotLightsConstant[i] = nullptr;
        m_basePassSpotLightsQuadratic[i] = nullptr;
        m_basePassSpotLightsInnerCut[i] = nullptr;
        m_basePassSpotLightsOutCut[i] = nullptr;
    }
    
    m_basePassPointLightsCount = nullptr;
    m_basePassSpotLightsCount = nullptr;
    
    // Light objects PASS
    m_lightObjectMesh = nullptr;
    m_spotLightMesh = nullptr;
    
    m_lightObjectMatrixModelUniform = nullptr;
    m_lightObjectMatrixViewUniform = nullptr;
    m_lightObjectMatrixProjectionUniform = nullptr;
    m_lightObjectColorUniform = nullptr;
}

void SceneRenderer::init()
{
    Renderer::init();
    m_renderPasses.resize(SceneRendererPasses::PASS_COUNT);

    for (size_t i = 0; i < SceneRendererPasses::PASS_COUNT; i++)
    {
        RenderPass* renderPass = nullptr;

        SceneRendererPasses pass = static_cast<SceneRendererPasses>(i);

        if (pass == BASE_PASS)
        {
            std::vector<std::string> uniformNames = {"u_modelMatrix", "u_viewMatrix", "u_projectionMatrix", "time", "u_gamma", "u_albedo", "u_lightColor", "u_ambientColor", "u_smoothness", "u_ambientStrength", "u_specularStrength", "u_cameraPosition", "u_pointLightsCount", "u_spotLightsCount"};
            
            for (size_t i = 0; i < MAX_POINT_LIGHTS; i++)
            {
                char locBuff[100] = { '\0' };
                
                snprintf(locBuff, sizeof(locBuff), "u_pointLights[%zu].color", i);
                std::string name = std::string(locBuff);
                uniformNames.push_back(name);
                
                snprintf(locBuff, sizeof(locBuff), "u_pointLights[%zu].position", i);
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
                
                snprintf(locBuff, sizeof(locBuff), "u_spotLights[%zu].linear", i);
                name = std::string(locBuff);
                uniformNames.push_back(name);
                
                snprintf(locBuff, sizeof(locBuff), "u_spotLights[%zu].quadratic", i);
                name = std::string(locBuff);
                uniformNames.push_back(name);
                
                snprintf(locBuff, sizeof(locBuff), "u_spotLights[%zu].constant", i);
                name = std::string(locBuff);
                uniformNames.push_back(name);
                
                snprintf(locBuff, sizeof(locBuff), "u_spotLights[%zu].innerCut", i);
                name = std::string(locBuff);
                uniformNames.push_back(name);
                
                snprintf(locBuff, sizeof(locBuff), "u_spotLights[%zu].outCut", i);
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
            m_basePassAlbedoUniform = renderPass->getUniform(uniformNames[5]);
            m_baseLightColorUniform = renderPass->getUniform(uniformNames[6]);
            m_basePassAmbientColorUniform = renderPass->getUniform(uniformNames[7]);
            m_basePasSmoothnessUniform = renderPass->getUniform(uniformNames[8]);
            m_basePassAmbientStrengthUniform = renderPass->getUniform(uniformNames[9]);
            m_basePassSpecularStrengthUniform = renderPass->getUniform(uniformNames[10]);
            m_basePassCameraPosition = renderPass->getUniform(uniformNames[11]);
            m_basePassPointLightsCount = renderPass->getUniform(uniformNames[12]);
            m_basePassSpotLightsCount = renderPass->getUniform(uniformNames[13]);
            
            for (size_t i = 0; i < MAX_POINT_LIGHTS; i++)
            {
                char locBuff[100] = { '\0' };
                
                snprintf(locBuff, sizeof(locBuff), "u_pointLights[%zu].color", i);
                std::string name = std::string(locBuff);
                m_basePassPointLightColor[i] = renderPass->getUniform(name);
                
                snprintf(locBuff, sizeof(locBuff), "u_pointLights[%zu].position", i);
                name = std::string(locBuff);
                m_basePassPointLightsPosition[i] = renderPass->getUniform(name);
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
                
                snprintf(locBuff, sizeof(locBuff), "u_spotLights[%zu].linear", i);
                name = std::string(locBuff);
                m_basePassSpotLightsLinear[i] = renderPass->getUniform(name);
                
                snprintf(locBuff, sizeof(locBuff), "u_spotLights[%zu].constant", i);
                name = std::string(locBuff);
                m_basePassSpotLightsConstant[i] = renderPass->getUniform(name);
                
                snprintf(locBuff, sizeof(locBuff), "u_spotLights[%zu].quadratic", i);
                name = std::string(locBuff);
                m_basePassSpotLightsQuadratic[i] = renderPass->getUniform(name);
                
                snprintf(locBuff, sizeof(locBuff), "u_spotLights[%zu].innerCut", i);
                name = std::string(locBuff);
                m_basePassSpotLightsInnerCut[i] = renderPass->getUniform(name);
                
                snprintf(locBuff, sizeof(locBuff), "u_spotLights[%zu].outCut", i);
                name = std::string(locBuff);
                m_basePassSpotLightsOutCut[i] = renderPass->getUniform(name);
            }
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
            std::vector<std::string> uniformNames = { "u_modelMatrix", "u_viewMatrix", "u_projectionMatrix", "u_color" };
            
            renderPass = new RenderPass();
            renderPass->makeProgram("light", "light");
            renderPass->registerUniforms(uniformNames);
            
            m_lightObjectMatrixModelUniform = renderPass->getUniform(uniformNames[0]);
            m_lightObjectMatrixViewUniform = renderPass->getUniform(uniformNames[1]);
            m_lightObjectMatrixProjectionUniform = renderPass->getUniform(uniformNames[2]);
            m_lightObjectColorUniform =renderPass->getUniform(uniformNames[3]);
        }
        
        if (pass == POSTPROCESSING_PASS)
        {
            std::vector<std::string> uniformNames = { "u_texture_0", "u_chromaticAberration", "u_sepia", "u_filmGrain", "u_vignette", "u_time", "u_gamma"};
            
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
        }

        m_renderPasses[i] = renderPass;
    }
    
    m_frameBuffer = Render::createFrameBuffer();
    m_lightObjectMesh = loadMesh("light.mesh");
    m_spotLightMesh = loadMesh("spot.mesh");
}

void SceneRenderer::finish()
{
    Renderer::finish();
}

void SceneRenderer::renderBasePass(std::vector<PointLightData>& lights, std::vector<SpotLightData>& spots, RenderInfo& renderInfo)
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

    m_basePassGammaUniform->setFloat(m_gamma);
    m_basePassAlbedoUniform->setVec3(m_basePassAlbedo);
    m_baseLightColorUniform->setVec3(m_basePassLightColor);
    m_basePassAmbientColorUniform->setVec3(m_basePassAmbientColor);
    m_basePasSmoothnessUniform->setFloat(m_basePassSmoothness);
    m_basePassAmbientStrengthUniform->setFloat(m_basePassAmbientStrength);
    m_basePassSpecularStrengthUniform->setFloat(m_basePassSpecularStrength);
    m_basePassCameraPosition->setVec3(cameraPosition);
    
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
    
    glm::vec3 lColor = glm::vec3(1.0, .0, .0);
    
    //m_basePassPointLightColor[0]->setVec3(lColor);
    
    for (size_t i = 0; i < lightsCount; i++)
    {
        PointLightData data = lights[i];
        m_basePassPointLightColor[i]->setVec3(data.color);
        m_basePassPointLightsPosition[i]->setVec3(data.position);
    }
    
    for (size_t i = 0; i < spotLightCount; i++)
    {
        SpotLightData data = spots[i];
        m_basePassSpotLightsDirection[i]->setVec3(data.direction);
        m_basePassSpotLightsPosition[i]->setVec3(data.position);
        m_basePassSpotLightsColor[i]->setVec3(data.color);
        m_basePassSpotLightsLinear[i]->setFloat(data.linear);
        m_basePassSpotLightsConstant[i]->setFloat(data.constant);
        m_basePassSpotLightsQuadratic[i]->setFloat(data.quadratic);
        m_basePassSpotLightsOutCut[i]->setFloat(data.outCut);
        m_basePassSpotLightsInnerCut[i]->setFloat(data.innerCut);
    }
    
    for (size_t i = 0; i < numGameObject; i++)
    {
        GameObject* gameObject = scene->getGameObject(i);
        Mesh* mesh = gameObject->getMesh();

        if (!mesh || !gameObject->isRenderingObject())
        {
            continue;
        }

        glm::mat4 modelMatrix = gameObject->getModelMatrix();
        m_matrixModelUniform->setMatrix4x4(modelMatrix);

        Render::drawMesh(mesh, renderInfo);
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

    for (size_t i = 0; i < numGameObject; i++)
    {
        GameObject* gameObject = scene->getGameObject(i);
        Mesh* mesh = gameObject->getMesh();

        if (!mesh || !gameObject->isRenderingObject())
        {
            continue;
        }

        glm::mat4 modelMatrix = gameObject->getModelMatrix();
        m_boundMatrixModelUniform->setMatrix4x4(modelMatrix);

        Render::drawMeshBound(mesh->getMeshBound(), renderInfo);
    }

    Render::endRenderPass(renderPass);
}

void SceneRenderer::renderLightObjectsPass(std::vector<PointLightData>& lights, std::vector<SpotLightData>& spots, RenderInfo& renderInfo)
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
    
    Render::endRenderPass(renderPass);
}

void SceneRenderer::renderPostProcessingPass(RenderInfo& renderInfo)
{
    RenderPass* renderPass = m_renderPasses[SceneRendererPasses::POSTPROCESSING_PASS];
    ScreenRenderPass* pass = dynamic_cast<ScreenRenderPass*>(renderPass);

    Render::startRenderPass(renderPass, renderInfo);
    
    Texture* texture = m_frameBuffer->getColorTexture();
    m_sceneTextureUniform->setTexture(texture);
    
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
    
    constructLightsData(lights, spots);
    
    Render::useFrameBuffer(m_frameBuffer);

    float gamma = m_gamma;

    glm::vec3 sceneColor = glm::pow(m_sceneColor, glm::vec3(gamma));
    
    Render::clearView(sceneColor.r, sceneColor.g, sceneColor.b, 1.0);

    if (m_renderBasePass)
    {
        renderBasePass(lights, spots, renderInfo);
    }

    if (m_renderBoundPass)
    {
        renderBoundPass(renderInfo);
    }
    
    if (m_renderLightObjectsPass)
    {
        renderLightObjectsPass(lights, spots, renderInfo);
    }
    
    Render::unUseFrameBuffer();
    
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

    float* f = glm::value_ptr(m_boundColor);
    ImGui::ColorEdit3("bound", f);

    f = glm::value_ptr(m_sceneColor);
    ImGui::ColorEdit3("scene color", f);

    f = glm::value_ptr(m_basePassAlbedo);
    ImGui::ColorEdit3("albedo", f);

    f = glm::value_ptr(m_basePassLightColor);
    ImGui::ColorEdit3("light color", f);

    f = glm::value_ptr(m_basePassAmbientColor);
    ImGui::ColorEdit3("ambient color", f);

    ImGui::SliderFloat("Ambient Strength", &m_basePassAmbientStrength, .0f, 1.0f);
    ImGui::SliderFloat("Specular Strength", &m_basePassSpecularStrength, .0f, 1.0f);

    ImGui::SliderFloat("smoothness", &m_basePassSmoothness, .001f, 1.0f);

    ImGui::End();
}

void SceneRenderer::constructLightsData(std::vector<PointLightData>& lights, std::vector<SpotLightData>& spots)
{
    SceneManager* manager = Engine::get()->getSceneManager();
    Scene* scene = manager->getScene();

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
