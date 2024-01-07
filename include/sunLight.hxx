//
// Created by Eugene Karpenko @ CaptainGPU
// https://twitter.com/CaptainGPU
//

#pragma once

#include "gameObject.hxx"

struct SunLightData
{
    glm::vec3 direction;
    glm::vec3 color;
    float intensity;
    glm::mat4 model;
};

class SunLight : public GameObject
{
public:
    SunLight();
    
    void setColor(glm::vec3 color);
    glm::vec3 getColor();
    
    void setIntensity(float intensity);
    float getIntensity();
    
    static SunLightData getSunLightData(SunLight* light);
    
private:
    glm::vec3 m_color;
    float m_intensity;
};
