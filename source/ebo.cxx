//
// Created by Eugene Karpenko @ CaptainGPU
// https://twitter.com/CaptainGPU
//

#include "ebo.hxx"
#include "render.hxx"

ElementBufferObject::ElementBufferObject(uint32_t numIndices)
:m_ebo(0),
m_numIndices(numIndices)
{
    
}

void ElementBufferObject::set_OpenGL_EBO(GLuint ebo)
{
    m_ebo = ebo;
}

GLuint ElementBufferObject::get_OpenGL_EBO()
{
    return m_ebo;
}

void ElementBufferObject::init()
{
    Render::createEBO(this);
}

uint32_t ElementBufferObject::getNumIndices()
{
    return m_numIndices;
}
