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
    
    for (size_t i = 0; i < numGameObject; i++)
    {
        GameObject* gameObject = scene->getGameObject(i);
    }
    
    Render::clearView(1.0, .0, .0, 1.0);

    Render::draw();
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
        }

        m_renderPasses[i] = renderPass;
    }

    Render::createTriangle();
    Render::compileShaders();
}

void SceneRenderer::finish()
{
    Renderer::finish();
}
