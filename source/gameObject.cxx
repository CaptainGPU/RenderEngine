//
// Created by Eugene Karpenko @ CaptainGPU
// https://twitter.com/CaptainGPU
//
#include "gameObject.hxx"

#include <iostream>

const float toRadians = 3.14159265f / 180.0f;

GameObject::GameObject(std::string name):
m_isDirtyTransform(true),
m_renderingObject(true),
m_mesh(nullptr),
m_lightComponent(nullptr)
{
    m_name = name;
    
    m_position = glm::vec3(.0);
    m_scale = glm::vec3(1.);
    m_rotation = glm::vec3(.0);
}


void GameObject::startPlay()
{
    //printf("GameObject: %s, start play!\n", m_name.c_str());
}

void GameObject::endPlay()
{
    if (m_mesh)
    {
        //m_mesh->finish();
        //delete m_mesh;
        m_mesh = nullptr;
    }

    printf("GameObject: %s, end play!\n", m_name.c_str());
}

void GameObject::update(float deltaTime)
{
}

void GameObject::drawGUI()
{
    
}

glm::mat4 GameObject::getModelMatrix()
{
    if (m_isDirtyTransform)
    {
        m_modelMatrix = glm::mat4(1.0f);
        m_modelMatrix = glm::translate(m_modelMatrix, m_position);
        
        
        m_modelMatrix = glm::rotate(m_modelMatrix, glm::radians(m_rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
        m_modelMatrix = glm::rotate(m_modelMatrix, glm::radians(m_rotation.y), glm::vec3(0.0f, 1.0f, .0f));
        m_modelMatrix = glm::rotate(m_modelMatrix, glm::radians(m_rotation.x), glm::vec3(1.0f, 0.0f, .0f));
        m_modelMatrix = glm::scale(m_modelMatrix, m_scale);
        
        glm::mat4 m = glm::mat4(1.0f);
        m = glm::translate(m, glm::vec3(0.25, 0.0, 0.0));
        
        //m_modelMatrix = m * m_modelMatrix;
        
        m_isDirtyTransform = false;
    }
    
    return m_modelMatrix;
}

void GameObject::addMesh(Mesh* mesh)
{
    m_meshes.push_back(mesh);
}

Mesh* GameObject::getMesh(unsigned int id)
{
    return m_meshes[id];
}

unsigned int GameObject::getMeshCount()
{
    return m_meshes.size();
}

bool GameObject::isRenderingObject()
{
    return m_renderingObject;
}

void GameObject::setIsRenderingObject(bool isRenderingObject)
{
    m_renderingObject = isRenderingObject;
}

// Transform staff

glm::vec3 GameObject::getPosition()
{
    return m_position;
}

void GameObject::addPosition(const glm::vec3& position)
{
    m_position += position;
    m_isDirtyTransform = true;
}

void GameObject::setPosition(const glm::vec3& position)
{
    m_position = position;
    m_isDirtyTransform = true;
}

void GameObject::setPositionX(const float& x)
{
    m_position.x = x;
    m_isDirtyTransform = true;
}

void GameObject::setPositionY(const float& y)
{
    m_position.y = y;
    m_isDirtyTransform = true;
}

void GameObject::setPositionZ(const float& z)
{
    m_position.z = z;
    m_isDirtyTransform = true;
}

void GameObject::addRotationX(const float& x)
{
    m_rotation.x += x;
    
    if (m_rotation.x > 360.0)
    {
        m_rotation.x -= 360.0;
    }
    
    m_isDirtyTransform = true;
}

void GameObject::addRotationY(const float& y)
{
    m_rotation.y += y;
    
    if (m_rotation.y > 360.0)
    {
        m_rotation.y -= 360.0;
    }
    
    m_isDirtyTransform = true;
}

void GameObject::addRotationZ(const float& z)
{
    m_rotation.z += z;
    
    if (m_rotation.z > 360.0)
    {
        m_rotation.z -= 360.0;
    }
    
    m_isDirtyTransform = true;
}

void GameObject::SetRotateX(const float& x)
{
    m_rotation.x = x;
    
    if (m_rotation.x > 360.0)
    {
        m_rotation.x -= 360.0;
    }
    
    m_isDirtyTransform = true;
}

void GameObject::SetRotateY(const float& y)
{
    m_rotation.y = y;
    
    if (m_rotation.y > 360.0)
    {
        m_rotation.y -= 360.0;
    }
    
    m_isDirtyTransform = true;
}

void GameObject::SetRotateZ(const float& z)
{
    m_rotation.z = z;
    
    if (m_rotation.z > 360.0)
    {
        m_rotation.z -= 360.0;
    }
    
    m_isDirtyTransform = true;
}

void GameObject::setRotation(const glm::vec3& rotation)
{
    m_rotation = rotation;
    m_isDirtyTransform = true;
}

float GameObject::getRotationX()
{
    return m_rotation.x;
}

float GameObject::getRotationY()
{
    return m_rotation.y;
}

float GameObject::getRotationZ()
{
    return m_rotation.z;
}

void GameObject::setScale(const glm::vec3& scale)
{
    m_scale = scale;
}

// Light staff

LightComponent* GameObject::getLightComponent()
{
    return m_lightComponent;
}
