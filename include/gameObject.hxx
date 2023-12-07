//
// Created by Eugene Karpenko @ CaptainGPU
// https://twitter.com/CaptainGPU
//
#pragma once

#include <glm/glm.hpp>

class GameObject
{
public:
    GameObject();
    
private:
    glm::vec3 m_position;
    glm::vec3 m_scale;
    glm::vec3 m_rotation;
};
