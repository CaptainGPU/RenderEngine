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

void Scene::simulate(float deltaTime)
{
    
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
    printf("%s scene has start play\n", m_name.c_str());
}

void Scene::endPlay()
{
    printf("%s scene has say goodbye!\n", m_name.c_str());
}
