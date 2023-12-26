//
// Created by Eugene Karpenko @ CaptainGPU
// https://twitter.com/CaptainGPU
//

#include "pointLightGameObject.hxx"
#include "pointLightComponent.hxx"

PoinLightGameObject::PoinLightGameObject():
GameObject("Point Light")
{
    m_lightComponent = new PointLightComponent();
}

void PoinLightGameObject::setColor(glm::vec3 color)
{
    m_color = color;
}

PointLightData PoinLightGameObject::getData()
{
    PointLightData data;
    data.color = m_color;
    data.model = getModelMatrix();
    data.position = getPosition();
    
    data.constant = 1.0f;
    data.linear = 0.7f;
    data.quadratic = 1.8;
    
    return data;
}
