//
// Created by Eugene Karpenko @ CaptainGPU
// https://twitter.com/CaptainGPU
//
#pragma once

#include <glm/glm.hpp>

class LightComponent
{
public:
    LightComponent();
    virtual ~LightComponent();

protected:
    glm::vec3 m_lightColor;
    
};
