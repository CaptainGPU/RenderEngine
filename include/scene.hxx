//
// Created by Eugene Karpenko @ CaptainGPU
// https://twitter.com/CaptainGPU
//
#pragma once

#include "gameObject.hxx"

#include <vector>
#include <string>

class Scene
{
public:
    Scene(std::string name);
    Scene (const Scene&) = delete;
    Scene& operator= (const Scene&) = delete;
    
    virtual ~Scene() = default;
    void simulate(float deltaTime);
    void addGameObject(GameObject* gameObject);
    
    virtual void startPlay();
    virtual void endPlay();
protected:
    std::vector<GameObject*> m_gameObjects;
    
private:
    std::string m_name;
};
