//
// Created by Eugene Karpenko @ CaptainGPU
// https://twitter.com/CaptainGPU
//
#pragma once

#include "renderPass.hxx"
#include "renderInfo.hxx"

#include <string>
#include <vector>

class Renderer {
public:
    Renderer(std::string name);
    virtual ~Renderer();
    
    virtual void init();
    virtual void render(RenderInfo& renderInfo) = 0;
    virtual void finish();

    virtual void drawDebugUI();
protected:
    std::vector<RenderPass*> m_renderPasses;
    
private:
    std::string m_name;
};
