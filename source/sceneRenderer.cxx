//
// Created by Eugene Karpenko @ CaptainGPU
// https://twitter.com/CaptainGPU
//
#include "sceneRenderer.hxx"

#include "engine.hxx"

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
    
}
