//
// Created by Eugene Karpenko @ CaptainGPU
// https://twitter.com/CaptainGPU
//
#pragma once

#include "mesh.hxx"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>

class GameObject
{
public:
    GameObject(std::string name);
    GameObject (const GameObject&) = delete;
    GameObject& operator= (const GameObject&) = delete;
    
    virtual void startPlay();
    virtual void endPlay();
    
    virtual void update(float deltaTime);
    
    virtual void drawGUI();
    
    glm::mat4 getModelMatrix();

    Mesh* getMesh();
    
    // Transform staff
    
    void setPositionY(const float& y);
    
private:
    glm::vec3 m_position;
    glm::vec3 m_scale;
    glm::vec3 m_rotation;
    
    glm::mat4 m_modelMatrix;
    
    std::string m_name;
    
    bool m_isDirtyModelMatrix;

protected:
    Mesh* m_mesh;
};
