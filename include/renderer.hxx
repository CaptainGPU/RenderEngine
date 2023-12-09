//
// Created by Eugene Karpenko @ CaptainGPU
// https://twitter.com/CaptainGPU
//
#pragma once

#include "renderPass.hxx"

#include <string>
#include <vector>

class Renderer {
public:
    Renderer(std::string name);
    virtual ~Renderer();
    
    virtual void init();
    virtual void render() = 0;
    virtual void finish();
protected:
    std::vector<RenderPass*> m_renderPasses;
    
private:
    std::string m_name;
};
