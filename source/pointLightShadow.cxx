//
// Created by Eugene Karpenko @ CaptainGPU
// https://twitter.com/CaptainGPU
//

#include "pointLightShadow.hxx"

#include "renderPass.hxx"
#include "frameBuffer.hxx"
#include "render.hxx"
#include "pointLightGameObject.hxx"
#include "engine.hxx"

#include <string>

const int POINTLIGHT_SHADOWMAP_WIDTH = 512;
const int POINTLIGHT_SHADOWMAP_HEIGHT = POINTLIGHT_SHADOWMAP_WIDTH;
const int POINTLIGHT_SIDES = 6;
const int POINTLIGHT_SHADOW_COUNT = 4;

Uniform* plModelMatrixUniform = nullptr;
Uniform* plProjectionMatrixUniform = nullptr;
Uniform* plLightPositionUniform = nullptr;
Uniform* plLightRadiusUniform = nullptr;
FrameBuffer* pointLightShadowMapFrameBuffer[4];
glm::mat4 plProjectionMatrix;

RenderPass* registerPointLightShadowPass()
{
    std::vector<std::string> uniformNames = { "u_modelMatrix", "u_projectionMatrix", "u_lightPosition", "u_lightRadius"};

    RenderPass* renderPass = new RenderPass();
    renderPass->makeProgram("pointLightShadow", "pointLightShadow");
    renderPass->registerUniforms(uniformNames);

    plModelMatrixUniform = renderPass->getUniform(uniformNames[0]);
    plProjectionMatrixUniform = renderPass->getUniform(uniformNames[1]);
    plLightPositionUniform = renderPass->getUniform(uniformNames[2]);
    plLightRadiusUniform = renderPass->getUniform(uniformNames[3]);

    plProjectionMatrix = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 100.0f);

    for (int i = 0; i < POINTLIGHT_SHADOW_COUNT; i++)
    {
        pointLightShadowMapFrameBuffer[i] = Render::createDepthCubeMapFrameBuffer(POINTLIGHT_SHADOWMAP_WIDTH, POINTLIGHT_SHADOWMAP_HEIGHT);
    }

    return renderPass;
}

void renderSide(unsigned int side, RenderInfo& renderInfo, PointLightData& lightData)
{
    SceneManager* manager = Engine::get()->getSceneManager();
    Scene* scene = manager->getScene();
    Camera* camera = scene->getCamera();

    size_t numGameObject = scene->getGameObjectCount();

    glm::vec3 dirs[POINTLIGHT_SIDES] = { 
        glm::vec3(1.0, 0.0, 0.0), 
        glm::vec3(-1.0, 0.0, 0.0),
        glm::vec3(0.0, 0.0, 1.0),
        glm::vec3(0.0, 0.0,-1.0),
        glm::vec3(0.0, 1.0, 0.0),
        glm::vec3(0.0,-1.0, 0.0),
    };

    glm::vec3 ups[POINTLIGHT_SIDES] = {
        glm::vec3(0.0, -1.0, 0.0),
        glm::vec3(0.0, -1.0, 0.0),
        glm::vec3(0.0, -1.0, 0.0),
        glm::vec3(0.0, -1.0, 0.0),
        glm::vec3(0.0, 0.0,-1.0),
        glm::vec3(0.0, 0.0,-1.0),
    };

    glm::vec3 front = dirs[side];
    glm::vec3 up = ups[side];

    glm::mat4 view = glm::lookAt(lightData.position, lightData.position + front, up);
    glm::mat4 matrix = plProjectionMatrix * view;

    plProjectionMatrixUniform->setMatrix4x4(matrix);

    glm::vec3 lightPos = lightData.position;
    plLightPositionUniform->setVec3(lightPos);

    float radius = lightData.radius;
    plLightRadiusUniform->setFloat(radius);

    for (size_t i = 0; i < numGameObject; i++)
    {
        GameObject* gameObject = scene->getGameObject(i);
        Mesh* mesh = gameObject->getMesh();

        if (!mesh || !gameObject->isRenderingObject())
        {
            continue;
        }

        glm::mat4 modelMatrix = gameObject->getModelMatrix();
        plModelMatrixUniform->setMatrix4x4(modelMatrix);

        Render::drawMesh(mesh, renderInfo);
    }
}

void renderPointLight(size_t index, RenderPass* renderPass, RenderInfo& renderInfo, PointLightData& lightData)
{
    FrameBuffer* frameBuffer = pointLightShadowMapFrameBuffer[index];

    for (size_t side = 0; side < POINTLIGHT_SIDES; side++)
    {
        Render::startRenderPass(renderPass, renderInfo);

        Render::useFrameBuffer(frameBuffer);

        Render::bindCubeMapTexture(frameBuffer->getCubeTexture());

        Render::setFrameBufferCubeSideRender(frameBuffer, side);

        Render::unBindCubeMapTexture();

        Render::clearView(1.0, 1.0, 1.0, 1.0);

        renderSide(side, renderInfo, lightData);

        Render::unUseFrameBuffer();

        Render::endRenderPass(renderPass);
    }
}

void renderPointLightShadowsPass(RenderPass* renderPass, RenderInfo& renderInfo, std::vector<PointLightData>& points, int shadowCount)
{
    Render::setViewport(0, 0, POINTLIGHT_SHADOWMAP_WIDTH, POINTLIGHT_SHADOWMAP_HEIGHT);

    size_t pointLightCount = shadowCount;
    
    if (pointLightCount > POINTLIGHT_SHADOW_COUNT)
    {
        pointLightCount = POINTLIGHT_SHADOW_COUNT;
    }

    for (size_t i = 0; i < pointLightCount; i++)
    {
        renderPointLight(i, renderPass, renderInfo, points[i]);
    }
}

std::vector<Texture*> getPointLightShadowMapTexture()
{
    std::vector<Texture*> textures;
    textures.reserve(4);

    for (size_t i = 0; i < POINTLIGHT_SHADOW_COUNT; i++)
    {
        Texture* tex = pointLightShadowMapFrameBuffer[i]->getCubeDepthTexture();
        textures.push_back(tex);
    }

    return textures;
}

std::vector<Texture*> getPointLightShadowMapColorTexture()
{
    std::vector<Texture*> textures;
    textures.reserve(4);

    for (size_t i = 0; i < POINTLIGHT_SHADOW_COUNT; i++)
    {
        Texture* tex = pointLightShadowMapFrameBuffer[i]->getCubeTexture();
        textures.push_back(tex);
    }

    return textures;
}
