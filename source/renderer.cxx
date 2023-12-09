//
// Created by Eugene Karpenko @ CaptainGPU
// https://twitter.com/CaptainGPU
//
#include "renderer.hxx"

Renderer::Renderer(std::string name)
{
    m_name = name;
}

Renderer::~Renderer()
{
}

void Renderer::init()
{
    printf("Renderer: %s init!\n", m_name.c_str());
}

void Renderer::finish()
{
    printf("Renderer: %s say goodbye!\n", m_name.c_str());
}
