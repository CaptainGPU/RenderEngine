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
}

void GameObject::startPlay()
{
    printf("GameObject: %s, start play!\n", m_name.c_str());
}

void GameObject::endPlay()
{
    printf("GameObject: %s, end play!\n", m_name.c_str());
}

void GameObject::update(float deltaTime)
{
}

void GameObject::drawGUI()
{
    
}
