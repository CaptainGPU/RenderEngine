//
// Created by Eugene Karpenko @ CaptainGPU
// https://twitter.com/CaptainGPU
//

#include "renderGUI.hxx"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

// UI Rendering start function
void RenderGUI::starRender()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

// Temporary function for displaying UI elements, TODO: Will be deleted
void RenderGUI::frame()
{
    static bool showDemo = false;
    ImGui::Begin("Example");
    if (ImGui::Button("Show/Hide ImGui demo"))
        showDemo = !showDemo;
    ImGui::End();
    if (showDemo)
        ImGui::ShowDemoWindow(&showDemo);
}

// UI Rendering end function
void RenderGUI::endRender()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
