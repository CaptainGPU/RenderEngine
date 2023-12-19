//
// Created by Eugene Karpenko @ CaptainGPU
// https://twitter.com/CaptainGPU
//
#include "sceneRenderer.hxx"
#include "engine.hxx"
#include "render.hxx"
#include "imgui.h"
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
m_renderBoundPass(true),
m_renderBasePass(true),
m_boundColor(1.0)
{
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
            std::vector<std::string> uniformNames = {"u_modelMatrix", "u_viewMatrix", "u_projectionMatrix", "time"};
            
            renderPass = new RenderPass();
            renderPass->makeProgram("mesh", "mesh");
            renderPass->registerUniforms(uniformNames);
            
            m_matrixModelUniform = renderPass->getUniform(uniformNames[0]);
            m_matrixViewUniform = renderPass->getUniform(uniformNames[1]);
            m_matrixProjectionUniform = renderPass->getUniform(uniformNames[2]);
            m_timeUniform = renderPass->getUniform(uniformNames[3]);
        }

        if (pass == BOUND_PASS)
        {
            std::vector<std::string> uniformNames = { "u_modelMatrix", "u_viewMatrix", "u_projectionMatrix", "u_boundColor"};

            renderPass = new RenderPass();
            renderPass->setWireFrameRender(true);
            renderPass->setTwoSideRender(true);
            renderPass->makeProgram("meshBound", "meshBound");
            renderPass->registerUniforms(uniformNames);

            m_boundMatrixModelUniform = renderPass->getUniform(uniformNames[0]);
            m_boundMatrixViewUniform = renderPass->getUniform(uniformNames[1]);
            m_boundMatrixProjectionUniform = renderPass->getUniform(uniformNames[2]);
            m_boundColorUniform = renderPass->getUniform(uniformNames[3]);
        }

        m_renderPasses[i] = renderPass;
    }

    //Render::init();
}

void SceneRenderer::finish()
{
    Renderer::finish();
}

void SceneRenderer::renderBasePass(RenderInfo& renderInfo)
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

void SceneRenderer::render(RenderInfo& renderInfo)
{
    Render::clearView(164.0f / 256.0f, 189.0f / 256.0f, 191.0f / 256.0f, 1.0);

    if (m_renderBasePass)
    {
        renderBasePass(renderInfo);
    }

    if (m_renderBoundPass)
    {
        renderBoundPass(renderInfo);
    }
}

void SceneRenderer::drawDebugUI()
{
    ImGui::Begin("Scene Renderer");
    ImGui::Checkbox("Base Pass", &m_renderBasePass);
    ImGui::Checkbox("Bound Pass", &m_renderBoundPass);

    float* f = glm::value_ptr(m_boundColor);
    ImGui::ColorEdit3("", f);

    ImGui::End();
}