//
// Created by Eugene Karpenko @ CaptainGPU
// https://twitter.com/CaptainGPU
//
#include "sceneRenderer.hxx"
#include "engine.hxx"
#include "render.hxx"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

SceneRenderer::SceneRenderer()
:Renderer("SceneRenderer"),
m_matrixModelUniform(nullptr)
{
}

void SceneRenderer::render()
{
    SceneManager* manager = Engine::get()->getSceneManager();
    Scene* scene = manager->getScene();

    size_t numGameObject = scene->getGameObjectCount();

    
    RenderPass* renderPass = m_renderPasses[SceneRendererPasses::TRIANGLE_PASS];

    
    Render::startRenderPass(renderPass);

    //Render::clearView(1.0, .0, .0, 1.0);
    for (size_t i = 0; i < numGameObject; i++)
    {
        GameObject* gameObject = scene->getGameObject(i);
        Mesh* mesh = gameObject->getMesh();

        //Render::createVBO(mesh, m_meshVAO);

        glm::mat4 modelMatrix = gameObject->getModelMatrix();
        m_matrixModelUniform->setMatrix4x4(modelMatrix);
        Render::drawMesh(mesh);
    }

    Render::endRenderPass(renderPass);
    
    //Render::draw();
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
            std::vector<std::string> uniformNames = {"modelMatrix"};
            
            renderPass = new RenderPass();
            renderPass->makeProgram();
            renderPass->registerUniforms(uniformNames);
            
            m_matrixModelUniform = renderPass->getUniform(uniformNames[0]);
        }

        m_renderPasses[i] = renderPass;
    }

    //Render::init();
}

void SceneRenderer::finish()
{
    Renderer::finish();
}
