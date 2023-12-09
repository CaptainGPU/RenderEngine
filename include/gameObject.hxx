//
// Created by Eugene Karpenko @ CaptainGPU
// https://twitter.com/CaptainGPU
//
#pragma once

#include "mesh.hxx"

#include <glm/glm.hpp>
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

    Mesh* getMesh();
    
private:
    glm::vec3 m_position;
    glm::vec3 m_scale;
    glm::vec3 m_rotation;
    
    std::string m_name;

protected:
    Mesh* m_mesh;
};
