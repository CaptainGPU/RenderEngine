//
// Created by Eugene Karpenko @ CaptainGPU
// https://twitter.com/CaptainGPU
//
#pragma once

#include "gameObject.hxx"
#include "camera.hxx"

#include <vector>
#include <string>

class SunLight;

class Scene
{
public:
    Scene(std::string name);
    Scene (const Scene&) = delete;
    Scene& operator= (const Scene&) = delete;
    
    virtual ~Scene() = default;
    void simulate(float deltaTime);
    void addGameObject(GameObject* gameObject);
    void drawGUI();
    
    size_t getGameObjectCount();
    GameObject* getGameObject(size_t index);

    Camera* getCamera();
    void setCamera(Camera* camera);
    
    virtual void construct();
    virtual void startPlay();
    virtual void endPlay();
    
    virtual void update(float deltaTime);
    
    void finish();
    
    SunLight* getSunLight();
    void setSunLight(SunLight* sun);
private:
    std::vector<GameObject*> m_gameObjects;

    Camera* m_defaultCamera;
    Camera* m_camera;
    
    SunLight* m_sunLight;
    
private:
    std::string m_name;
};
