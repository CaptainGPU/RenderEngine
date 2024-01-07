//
// Created by Eugene Karpenko @ CaptainGPU
// https://twitter.com/CaptainGPU
//

#include "sunLight.hxx"

#include "glm/gtx/rotate_vector.hpp"

SunLight::SunLight():
GameObject("Sun Light")
{
    m_color = glm::vec3(1.0, 1.0, .0);
    m_intensity = 1.0;
}

void SunLight::setColor(glm::vec3 color)
{
    m_color = color;
}

glm::vec3 SunLight::getColor()
{
    return m_color;
}

void SunLight::setIntensity(float intensity)
{
    m_intensity = intensity;
}

float SunLight::getIntensity()
{
    return m_intensity;
}

SunLightData SunLight::getSunLightData(SunLight* light)
{
    SunLightData data;
    
    if (light)
    {
        data.color = light->getColor();
        
        glm::vec3 forw = glm::vec3(1.0, .0, .0);
        
        forw = glm::rotate(forw, glm::radians(light->getRotationZ()), glm::vec3(0.0, .0, 1.0));
        forw = glm::rotate(forw, glm::radians(light->getRotationY()), glm::vec3(0.0, 1.0, .0));
        forw = glm::rotate(forw, glm::radians(light->getRotationX()), glm::vec3(1.0, .0, .0));
        
        data.direction = forw;
        data.intensity = light->getIntensity();
        data.model = light->getModelMatrix();
    }
    else
    {
        data.color = glm::vec3(1.0f, 1.0f, .0f);
        data.direction = glm::vec3(1.0f, .0f, .0f);
        data.intensity = 0.0f;
    }
    
    return data;
}
