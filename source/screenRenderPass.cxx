//
// Created by Eugene Karpenko @ CaptainGPU
// https://twitter.com/CaptainGPU
//

#include "screenRenderPass.hxx"
#include "meshLoader.hxx"
#include "render.hxx"

ScreenRenderPass::ScreenRenderPass():
RenderPass(),
m_mesh(nullptr)
{
    m_mesh = loadMesh("quad.mesh");
}

void ScreenRenderPass::draw(RenderInfo& renderInfo)
{
    Render::drawMesh(m_mesh, renderInfo);
}
