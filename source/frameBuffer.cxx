//
// Created by Eugene Karpenko @ CaptainGPU
// https://twitter.com/CaptainGPU
//

#include "frameBuffer.hxx"
#include <stdio.h>


FrameBuffer::FrameBuffer():
m_openGL_FBO(0),
m_depthTexture(nullptr),
m_cubeMapTexture(nullptr),
m_cubeDepthTexture(nullptr)
{
    m_attacmhentCount = 0;
    m_colorTexture = nullptr;
    m_colorTexture1 = nullptr;
    m_colorTexture2 = nullptr;

    printf("Render: create Frame Buffer \n");
}

void FrameBuffer::setColorTexture(Texture* texture)
{
    m_colorTexture = texture;
}

Texture* FrameBuffer::getColorTexture()
{
    return m_colorTexture;
}

void FrameBuffer::setColorTexture1(Texture* texture)
{
    m_colorTexture1 = texture;
}

Texture* FrameBuffer::getColorTexture1()
{
    return m_colorTexture1;
}

void FrameBuffer::setColorTexture2(Texture* texture)
{
    m_colorTexture2 = texture;
}

Texture* FrameBuffer::getColorTexture2()
{
    return m_colorTexture2;
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

void FrameBuffer::setCubeTexture(Texture* texture)
{
    m_cubeMapTexture = texture;
}

Texture* FrameBuffer::getCubeTexture()
{
    return m_cubeMapTexture;
}

void FrameBuffer::setCubeDepthTexture(Texture* texture)
{
    m_cubeDepthTexture = texture;
}

Texture* FrameBuffer::getCubeDepthTexture()
{
    return m_cubeDepthTexture;
}

void FrameBuffer::setAttachmentCount(unsigned int count)
{
    m_attacmhentCount = count;
}

unsigned int FrameBuffer::getAttachmentCount()
{
    return m_attacmhentCount;
}

unsigned int FrameBuffer::get_openGL_FBO()
{
    return m_openGL_FBO;
}
