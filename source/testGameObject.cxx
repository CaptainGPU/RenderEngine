//
// Created by Eugene Karpenko @ CaptainGPU
// https://twitter.com/CaptainGPU
//
#include "testGameObject.hxx"

#include <imgui.h>


TestGameObject::TestGameObject()
:GameObject("TestGameObject")
{
    m_deltaTime = .0;
    m_mesh = new Mesh();
}

void TestGameObject::update(float deltaTime)
{
    GameObject::update(deltaTime);
    m_deltaTime = deltaTime;
}

void TestGameObject::drawGUI()
{
    GameObject::drawGUI();
    
    ImGui::Begin("Example");
    
    ImGui::Text("RenderTime: %f(ms)", m_deltaTime);
    ImGui::Text("FPS: %f", 1.0f / m_deltaTime);
    
    ImGui::End();
}
