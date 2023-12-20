//
// Created by Eugene Karpenko @ CaptainGPU
// https://twitter.com/CaptainGPU
//

#pragma once

#include "renderPass.hxx"

class Mesh;
struct RenderInfo;

class ScreenRenderPass: public RenderPass
{
public:
    ScreenRenderPass();
    void draw(RenderInfo& renderInfo);
private:
    Mesh* m_mesh;
};
