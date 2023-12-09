//
// Created by Eugene Karpenko @ CaptainGPU
// https://twitter.com/CaptainGPU
//
#pragma once

#include <string>

class Renderer {
public:
    Renderer(std::string name);
    virtual ~Renderer();
    
    virtual void init();
    virtual void render() = 0;
    virtual void finish();
    
private:
    std::string m_name;
};
