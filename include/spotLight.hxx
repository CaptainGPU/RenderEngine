//
// Created by Eugene Karpenko @ CaptainGPU
// https://twitter.com/CaptainGPU
//

#pragma once

#include "gameObject.hxx"

struct SpotLightData
{
    glm::vec3 position;
    glm::vec3 color;
    glm::mat4 model;
    glm::vec3 direction;
    
    float constant;
    float linear;
    float quadratic;
    
    float innerCut;
    float outCut;
};

class SpotLight : public GameObject
{
public:
    SpotLight();
    SpotLightData getData();
    void setColor(glm::vec3 color);
    void setRange(float range);
    void setCutOffs(float inner, float out);
    
private:
    glm::vec3 m_color;
    
    float m_range;
    float m_constant;
    float m_linear;
    float m_quadratic;
    float m_innerCut;
    float m_outCut;
};
