#include "renderGUI.hxx"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>


void RenderGUI::starRender()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

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

void RenderGUI::endRender()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
