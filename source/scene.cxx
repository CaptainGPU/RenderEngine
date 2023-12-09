//
// Created by Eugene Karpenko @ CaptainGPU
// https://twitter.com/CaptainGPU
//
#include "scene.hxx"

#include <iostream>

Scene::Scene(std::string name)
{
    m_name = name;
}

void Scene::construct()
{
}

void Scene::simulate(float deltaTime)
{
    for(size_t i = 0; i < m_gameObjects.size(); i++)
    {
        GameObject* gameObject = m_gameObjects[i];
        
        if (!gameObject)
        {
            continue;
        }
        
        gameObject->update(deltaTime);
    }
}

void Scene::drawGUI()
{
    for(size_t i = 0; i < m_gameObjects.size(); i++)
    {
        GameObject* gameObject = m_gameObjects[i];
        
        if (!gameObject)
        {
            continue;
        }
        
        gameObject->drawGUI();
    }
}

size_t Scene::getGameObjectCount()
{
    return m_gameObjects.size();
}

GameObject* Scene::getGameObject(size_t index)
{
    return m_gameObjects[index];
}

void Scene::addGameObject(GameObject* gameObject)
{
    if (!gameObject) {
        printf("Try add invalid GameObject in to Scene!\n");
        return;
    }
    
    m_gameObjects.push_back(gameObject);
}

void Scene::startPlay()
{
    printf("%s scene has start play!\n", m_name.c_str());
    
    for(size_t i = 0; i < m_gameObjects.size(); i++)
    {
        GameObject* gameObject = m_gameObjects[i];
        
        if (!gameObject)
        {
            continue;
        }
        
        gameObject->startPlay();
    }
}

void Scene::endPlay()
{
    for(size_t i = 0; i < m_gameObjects.size(); i++)
    {
        GameObject* gameObject = m_gameObjects[i];
        
        if (!gameObject)
        {
            continue;
        }
        
        gameObject->endPlay();
    }
    
    
    printf("%s scene has say goodbye!\n", m_name.c_str());
}

void Scene::finish()
{
    for(size_t i = 0; i < m_gameObjects.size(); i++)
    {
        GameObject* gameObject = m_gameObjects[i];
        
        delete gameObject;
    }
}
