//
// Created by Eugene Karpenko @ CaptainGPU
// https://twitter.com/CaptainGPU
//
#include "sceneRenderer.hxx"
#include "engine.hxx"
#include "render.hxx"

SceneRenderer::SceneRenderer()
:Renderer("SceneRenderer")
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
            renderPass = new RenderPass();
            renderPass->makeProgram();
        }

        m_renderPasses[i] = renderPass;
    }

    //Render::init();
}

void SceneRenderer::finish()
{
    Renderer::finish();
}
