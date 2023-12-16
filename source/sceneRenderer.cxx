//
// Created by Eugene Karpenko @ CaptainGPU
// https://twitter.com/CaptainGPU
//
#include "sceneRenderer.hxx"
#include "engine.hxx"
#include "render.hxx"

SceneRenderer::SceneRenderer()
:Renderer("SceneRenderer"),
m_matrixModelUniform(nullptr),
m_matrixProjectionUniform(nullptr)
{
}

void SceneRenderer::render(RenderInfo& renderInfo)
{
    SceneManager* manager = Engine::get()->getSceneManager();
    Scene* scene = manager->getScene();
    Camera* camera = scene->getCamera();

    size_t numGameObject = scene->getGameObjectCount();
    
    RenderPass* renderPass = m_renderPasses[SceneRendererPasses::TRIANGLE_PASS];

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

void SceneRenderer::init()
{
    Renderer::init();
    m_renderPasses.resize(SceneRendererPasses::PASS_COUNT);

    for (size_t i = 0; i < SceneRendererPasses::PASS_COUNT; i++)
    {
        RenderPass* renderPass = nullptr;

        SceneRendererPasses pass = static_cast<SceneRendererPasses>(i);

        if (pass == TRIANGLE_PASS)
        {
            std::vector<std::string> uniformNames = {"u_modelMatrix", "u_viewMatrix", "u_projectionMatrix", "time"};
            
            renderPass = new RenderPass();
            renderPass->makeProgram();
            renderPass->registerUniforms(uniformNames);
            
            m_matrixModelUniform = renderPass->getUniform(uniformNames[0]);
            m_matrixViewUniform = renderPass->getUniform(uniformNames[1]);
            m_matrixProjectionUniform = renderPass->getUniform(uniformNames[2]);
            m_timeUniform = renderPass->getUniform(uniformNames[3]);
        }

        m_renderPasses[i] = renderPass;
    }

    //Render::init();
}

void SceneRenderer::finish()
{
    Renderer::finish();
}
