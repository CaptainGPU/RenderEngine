//
// Created by Eugene Karpenko @ CaptainGPU
// https://twitter.com/CaptainGPU
//
#include "testGameObject.hxx"

#include <imgui.h>

#include <glm/glm.hpp>


TestGameObject::TestGameObject()
:GameObject("TestGameObject"),
m_time(.0)
{
    m_deltaTime = .0;
    m_mesh = new Mesh();
}

void TestGameObject::update(float deltaTime)
{
    GameObject::update(deltaTime);
    /*m_deltaTime = deltaTime;
    
    m_time+=deltaTime;
    
    y = glm::sin(m_time);
    
    //setPositionZ(-2.0 - y);
    
    addRotationX(m_deltaTime);*/
}

void TestGameObject::drawGUI()
{
    GameObject::drawGUI();
    
    /*ImGui::Begin("Example");
    
    ImGui::Text("RenderTime: %f(ms)", m_deltaTime);
    ImGui::Text("FPS: %f", 1.0f / m_deltaTime);
    ImGui::Text("Time: %f", m_time);
    ImGui::Text("sin: %f", y);
    
    ImGui::End();*/
}
