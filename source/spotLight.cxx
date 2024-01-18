//
// Created by Eugene Karpenko @ CaptainGPU
// https://twitter.com/CaptainGPU
//

#include "spotLight.hxx"
#include "glm/gtx/rotate_vector.hpp"

SpotLight::SpotLight():
GameObject("Spot Light"),
m_color(glm::vec3(1.0f)),
m_range(.0f),
m_constant(.0f),
m_linear(.0f),
m_quadratic(.0f),
m_innerCut(.0f),
m_outCut(.0f),
m_outCutFov(0.0f)
{
    setRange(20.0f);
    setCutOffs(20.0f, 25.0f);
}

SpotLightData SpotLight::getData()
{

    SpotLightData data;
    
    data.model = getModelMatrix();
    data.color = m_color;
    data.position = getPosition();
    
    glm::vec3 forw = glm::vec3(1.0, .0, .0);

    forw = glm::rotate(forw, glm::radians(getRotationZ()), glm::vec3(0.0, .0, 1.0));
    forw = glm::rotate(forw, glm::radians(getRotationY()), glm::vec3(0.0, 1.0, .0));
    forw = glm::rotate(forw, glm::radians(getRotationX()), glm::vec3(1.0, .0, .0));
    
    data.direction = forw;
    
    data.constant = m_constant;
    data.linear = m_linear;
    data.quadratic = m_quadratic;
    
    data.innerCut = m_innerCut;
    data.outCut = m_outCut;
    data.FOV = m_outCutFov;
    
    return data;
}

void SpotLight::setColor(glm::vec3 color)
{
    m_color = color;
}

void SpotLight::setRange(float range)
{
    m_range = range;
    m_constant = 1.0f;
    m_linear = 4.5 / range;
    m_quadratic = 75.0f/(range*range);
}

void SpotLight::setCutOffs(float inner, float out)
{
    m_innerCut = glm::cos(glm::radians(inner));
    m_outCut = glm::cos(glm::radians(out));
    m_outCutFov = glm::radians(out * 2.0);
}
