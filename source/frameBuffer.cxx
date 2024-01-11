//
// Created by Eugene Karpenko @ CaptainGPU
// https://twitter.com/CaptainGPU
//

#include "frameBuffer.hxx"


FrameBuffer::FrameBuffer():
m_openGL_FBO(0),
m_colorTexture(nullptr),
m_depthTexture(nullptr)
{
    
}

void FrameBuffer::setColorTexture(Texture* texture)
{
    m_colorTexture = texture;
}

Texture* FrameBuffer::getColorTexture()
{
    return m_colorTexture;
}

void FrameBuffer::setDepthTexture(Texture* texture)
{
    m_depthTexture = texture;
}

Texture* FrameBuffer::getDepthTexture()
{
    return m_depthTexture;
}

void FrameBuffer::set_openGL_FBO(unsigned int fbo)
{
    m_openGL_FBO = fbo;
}

unsigned int FrameBuffer::get_openGL_FBO()
{
    return m_openGL_FBO;
}
