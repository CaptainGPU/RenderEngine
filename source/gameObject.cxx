//
// Created by Eugene Karpenko @ CaptainGPU
// https://twitter.com/CaptainGPU
//
#include "gameObject.hxx"


GameObject::GameObject()
{
    m_position = glm::vec3(.0);
    m_scale = glm::vec3(1.);
    m_rotation = glm::vec3(.0);
}
