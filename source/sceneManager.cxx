//
// Created by Eugene Karpenko @ CaptainGPU
// https://twitter.com/CaptainGPU
//
#include "sceneManager.hxx"

#include "testScene.hxx"

#include <iostream>

SceneManager::SceneManager()
{
}

void SceneManager::run()
{
    TestScene* scene = new TestScene();
    setScene(scene);
}

void SceneManager::setScene(Scene* scene)
{
    if (!scene)
    {
        printf("ERROR: SceneManager::setScene call with invalid Scene!\n");
        return;
    }
    
    m_scene = scene;
    
    m_scene->construct();
    m_scene->startPlay();
}

void SceneManager::releaseScene()
{
    if (m_scene)
    {
        m_scene->endPlay();
        delete m_scene;
    }
}

void SceneManager::simulate(float deltaTime)
{
    if (m_scene)
    {
        m_scene->simulate(deltaTime);
    }
}

void SceneManager::drawGUI()
{
    if (m_scene)
    {
        m_scene->drawGUI();
    }
}

void SceneManager::finish()
{
    releaseScene();
}

Scene* SceneManager::getScene()
{
    return m_scene;
}
