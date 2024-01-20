//
// Created by Eugene Karpenko @ CaptainGPU
// https://twitter.com/CaptainGPU
//

#include "spotLight.hxx"
#include "math.hxx"
#include "meshLoader.hxx"
#include "glm/gtx/rotate_vector.hpp"

SpotLight::SpotLight():
GameObject("Spot Light"),
m_color(glm::vec3(1.0f)),
m_range(.0f),
m_innerCut(.0f),
m_outCut(.0f),
m_outCutFov(0.0f),
m_lightBound(nullptr)
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
    
    data.FOV = m_outCutFov;
    data.range = m_range;
    data.invRange = 1.0 / m_range;

    glm::vec3 eye = data.position + glm::vec3(0.001f, 0.0f, 0.0f);
    glm::vec3 center = eye + data.direction;
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

    glm::mat4 viewMatrix = glm::lookAt(eye, center, up);
    glm::mat4 projMatrix = glm::perspective(data.FOV, 1.0f, 0.1f, data.range);

    glm::mat4 vpMatrix = projMatrix * viewMatrix;

    data.vpMatrix = vpMatrix;
    
    data.innerCut = m_innerCut;
    data.outCut = m_outCut;
    
    return data;
}

void SpotLight::setColor(glm::vec3 color)
{
    m_color = color;
}

glm::vec3 SpotLight::getColor()
{
    return m_color;
}

void SpotLight::setRange(float range)
{
    m_range = range;
}

void SpotLight::setCutOffs(float inner, float out)
{
    m_innerCut = glm::cos(glm::radians(inner));
    m_outCut = glm::cos(glm::radians(out));
    m_outCutFov = glm::radians(out * 2.0);
}

MeshBound* SpotLight::getMeshBound()
{
    return m_lightBound;
}

void SpotLight::generateBound()
{
    glm::vec3 eye = glm::vec3(0.0);
    glm::vec3 center = glm::vec3(1.0, 0.0, 0.0);
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

    glm::mat4 viewMatrix = glm::lookAt(eye, center, up);
    glm::mat4 projMatrix = glm::perspective(m_outCutFov, 1.0f, 0.1f, m_range);

    glm::mat4 space = projMatrix * viewMatrix;

    std::vector<glm::vec4> corners = getFrustumCornersWorldSpace(space);

    m_lightBound = createCorterBound(corners);
}
