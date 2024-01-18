//
// Created by Eugene Karpenko @ CaptainGPU
// https://twitter.com/CaptainGPU
//

#include "spotLightShadow.hxx"
#include "renderPass.hxx"
#include "uniform.hxx"
#include "spotLight.hxx"
#include "engine.hxx"

#include "render.hxx"

#include <string>

const int SPOTLIGHT_SHADOWMAP_WIDTH = 1024;
const int SPOTLIGHT_SHADOWMAP_HEIGHT = SPOTLIGHT_SHADOWMAP_WIDTH;

Uniform* modelMatrixUniform = nullptr;
Uniform* projectionMatrixUniform = nullptr;
FrameBuffer* spotLightShadowMapFrameBuffer[4];

glm::mat4 spotLightVPMatrix;

RenderPass* registerSpotLightShadowPass()
{
    std::vector<std::string> uniformNames = {"u_modelMatrix", "u_projectionMatrix"};

    RenderPass* renderPass = new RenderPass();
    renderPass->makeProgram("sunLightShadow", "sunLightShadow");
    renderPass->registerUniforms(uniformNames);

    modelMatrixUniform = renderPass->getUniform(uniformNames[0]);
    projectionMatrixUniform = renderPass->getUniform(uniformNames[1]);

    for (int i = 0; i < 4; i++)
    {
        spotLightShadowMapFrameBuffer[i] = Render::createDepthMapFrameBuffer();
    }

    return renderPass;
}

void renderSpotLight(SpotLightData& spotLightData, RenderInfo& renderInfo)
{
    SceneManager* manager = Engine::get()->getSceneManager();
    Scene* scene = manager->getScene();
    Camera* camera = scene->getCamera();

    size_t numGameObject = scene->getGameObjectCount();
    
    glm::vec3 eye = spotLightData.position + glm::vec3(0.001f, 0.0f, 0.0f);
    glm::vec3 center = eye + spotLightData.direction;
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
    
    glm::mat4 viewMatrix = glm::lookAt(eye, center, up);
    glm::mat4 projMatrix = glm::perspective(spotLightData.FOV, 1.0f, 0.1f, 30.0f);

    glm::mat4 vpMatrix = projMatrix * viewMatrix;

    spotLightData.vpMatrix = vpMatrix;
    spotLightVPMatrix = vpMatrix;

    projectionMatrixUniform->setMatrix4x4(vpMatrix);

    for (size_t i = 0; i < numGameObject; i++)
    {
        GameObject* gameObject = scene->getGameObject(i);
        Mesh* mesh = gameObject->getMesh();

        if (!mesh || !gameObject->isRenderingObject())
        {
            continue;
        }

        glm::mat4 modelMatrix = gameObject->getModelMatrix();
        modelMatrixUniform->setMatrix4x4(modelMatrix);

        Render::drawMesh(mesh, renderInfo);
    }
}

void renderSpotlightShadowsPass(RenderPass* renderPass, RenderInfo& renderInfo, std::vector<SpotLightData>& spots)
{
    Render::setViewport(0, 0, SPOTLIGHT_SHADOWMAP_WIDTH, SPOTLIGHT_SHADOWMAP_HEIGHT);

    size_t spotLightCount = spots.size();
    if (spotLightCount > 4)
    {
        spotLightCount = 4;
    }

    for (size_t i = 0; i < spotLightCount; i++)
    {
        Render::useFrameBuffer(spotLightShadowMapFrameBuffer[i]);

        Render::clearView(0.0, 0.0, 0.0, 1.0);

        Render::startRenderPass(renderPass, renderInfo);

        renderSpotLight(spots[i], renderInfo);

        Render::endRenderPass(renderPass);

        Render::unUseFrameBuffer();
    }
}

std::vector<Texture*> getSpotLightShadowMapTexture()
{
    std::vector<Texture*> textures;
    textures.reserve(4);

    for (size_t i = 0; i < 4; i++)
    {
        Texture* tex = spotLightShadowMapFrameBuffer[i]->getDepthTexture();
        textures.push_back(tex);
    }

    return textures;
}

std::vector<Texture*> getSpotLightShadowMapColorTexture()
{
    std::vector<Texture*> textures;
    textures.reserve(4);

    for (size_t i = 0; i < 4; i++)
    {
        Texture* tex = spotLightShadowMapFrameBuffer[i]->getColorTexture();
        textures.push_back(tex);
    }

    return textures;
}

