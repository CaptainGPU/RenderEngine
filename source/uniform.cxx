//
// Created by Eugene Karpenko @ CaptainGPU
// https://twitter.com/CaptainGPU
//

#include "uniform.hxx"

#include "render.hxx"

Uniform::Uniform()
:m_ogl_uniformID(0)
{
    
}

void Uniform::set_OpenGL_uniformID(GLint id)
{
    m_ogl_uniformID = id;
}

GLint Uniform::get_OpenGL_uniformID()
{
    return m_ogl_uniformID;
}

void Uniform::setVec3(glm::vec3& value)
{
    Render::setUniformVec3(this, value);
}

void Uniform::setMatrix4x4(glm::mat4& matrix)
{
    Render::setUniformMatrix4x4(this, matrix);
}

void Uniform::setFloat(float& value)
{
    Render::setUniformFloatValue(this, value);
}
