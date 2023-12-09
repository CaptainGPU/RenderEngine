//
// Created by Eugene Karpenko @ CaptainGPU
// https://twitter.com/CaptainGPU
//
#include "renderEngine.hxx"
#include "sceneRenderer.hxx"

#include <iostream>

void RenderEngine::init()
{
    printf("Render engine initialize!\n");
    m_renderer = new SceneRenderer();
    m_renderer->init();
}

void RenderEngine::render()
{
    clearRenderInfo(m_renderInfo);
    m_renderer->render();
}

void RenderEngine::finish()
{
    if (m_renderer)
    {
        m_renderer->finish();
        delete m_renderer;
        m_renderer = nullptr;
    }
    
    printf("Render engine say goodbye!\n");
}
