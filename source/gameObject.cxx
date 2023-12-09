//
// Created by Eugene Karpenko @ CaptainGPU
// https://twitter.com/CaptainGPU
//
#include "gameObject.hxx"

#include <iostream>


GameObject::GameObject(std::string name)
{
    m_name = name;
    
    m_position = glm::vec3(.0);
    m_scale = glm::vec3(1.);
    m_rotation = glm::vec3(.0);

    m_mesh = nullptr;
}

void GameObject::startPlay()
{
    printf("GameObject: %s, start play!\n", m_name.c_str());
}

void GameObject::endPlay()
{
    if (m_mesh)
    {
        m_mesh->finish();
        delete m_mesh;
        m_mesh = nullptr;
    }

    printf("GameObject: %s, end play!\n", m_name.c_str());
}

void GameObject::update(float deltaTime)
{
}

void GameObject::drawGUI()
{
    
}

Mesh* GameObject::getMesh()
{
    return m_mesh;
}
