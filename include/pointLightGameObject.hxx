//
// Created by Eugene Karpenko @ CaptainGPU
// https://twitter.com/CaptainGPU
//

#pragma once

#include "gameObject.hxx"

struct PointLightData
{
    glm::vec3 color;
    glm::mat4 model;
    glm::vec3 position;
    float radius;
    float invRadius;
    
    float constant;
    float linear;
    float quadratic;
};

class PoinLightGameObject : public GameObject
{
public:
    PoinLightGameObject();
    PointLightData getData();
    void setColor(glm::vec3 color);

    void setRadius(float radius);
    float getRadius();
    float getInvRadius();
    
private:
    glm::vec3 m_color;
    float m_radius;
    float m_invRadius;
};
