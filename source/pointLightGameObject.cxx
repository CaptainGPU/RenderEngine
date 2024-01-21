//
// Created by Eugene Karpenko @ CaptainGPU
// https://twitter.com/CaptainGPU
//

#include "pointLightGameObject.hxx"
#include "pointLightComponent.hxx"

PoinLightGameObject::PoinLightGameObject():
GameObject("Point Light"),
m_radius(0.0f),
m_invRadius(0.0f),
m_color(glm::vec3(1.0, 0.0, 0.0))
{
    m_lightComponent = new PointLightComponent();
    setRadius(10.0f);
}

void PoinLightGameObject::setColor(glm::vec3 color)
{
    m_color = color;
}

void PoinLightGameObject::setRadius(float radius)
{
    m_radius = radius;
    m_invRadius = 1.0 / radius;
}

float PoinLightGameObject::getRadius()
{
    return m_radius;
}

float PoinLightGameObject::getInvRadius()
{
    return m_invRadius;
}

PointLightData PoinLightGameObject::getData()
{
    PointLightData data;
    data.color = m_color;
    data.model = getModelMatrix();
    data.position = getPosition();

    data.radius = m_radius;
    data.invRadius = m_radius;
    
    data.constant = 1.0f;
    data.linear = 0.7f;
    data.quadratic = 1.8;
    
    return data;
}
