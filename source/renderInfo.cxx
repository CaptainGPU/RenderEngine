//
// Created by Eugene Karpenko @ CaptainGPU
// https://twitter.com/CaptainGPU
//
#include "renderInfo.hxx"

#include <imgui.h>

void clearRenderInfo(RenderInfo& ri)
{
	ri.numRenderObject = 0;
    ri.numIndices = 0;
    ri.numVertex = 0;
    ri.numRenderPasses = 0;
}

void addVertex(RenderInfo& ri, uint32_t num)
{
    ri.numVertex += num;
}

void addIndices(RenderInfo& ri, uint32_t num)
{
    ri.numIndices += num;
}
void addRenderPass(RenderInfo& ri)
{
    ri.numRenderPasses++;
}

void showStats(RenderInfo& ri)
{
    ImGui::Text("Render Passes: %u", ri.numRenderPasses);
    ImGui::Text("Render Objects: %u", ri.numRenderObject);
    ImGui::Text("Render Vertices: %u", ri.numVertex);
    ImGui::Text("Render Indices: %u", ri.numIndices);
}

void addRenderObject(RenderInfo& ri)
{
    ri.numRenderObject++;
}
