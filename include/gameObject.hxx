//
// Created by Eugene Karpenko @ CaptainGPU
// https://twitter.com/CaptainGPU
//
#pragma once

#include "mesh.hxx"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>

class LightComponent;

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

    Mesh* getMesh(unsigned int id);
    unsigned int getMeshCount();

    bool isRenderingObject();
    void setIsRenderingObject(bool isRenderingObject);
    
    // Transform staff

    glm::vec3 getPosition();
    
    void addPosition(const glm::vec3& position);
    void setPosition(const glm::vec3& position);
    void setPositionX(const float& x);
    void setPositionY(const float& y);
    void setPositionZ(const float& z);
    
    void addRotationX(const float& x);
    void addRotationY(const float& y);
    void addRotationZ(const float& z);
    
    void SetRotateX(const float& x);
    void SetRotateY(const float& y);
    void SetRotateZ(const float& z);
    
    float getRotationX();
    float getRotationY();
    float getRotationZ();
    
    void setScale(const glm::vec3& scale);
    
    // Light staff
    
    LightComponent* getLightComponent();
    
private:
    glm::vec3 m_position;
    glm::vec3 m_scale;
    glm::vec3 m_rotation;
    
    glm::mat4 m_modelMatrix;
    
    std::string m_name;
    
    bool m_isDirtyTransform;
    bool m_renderingObject;
    
protected:
    LightComponent* m_lightComponent;

protected:
    Mesh* m_mesh;
    std::vector<Mesh*> m_meshes;
};
