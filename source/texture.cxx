//
// Created by Eugene Karpenko @ CaptainGPU
// https://twitter.com/CaptainGPU
//

#include "texture.hxx"

Texture::Texture()
{
    
}

void Texture::set_OpenGL_Texture(unsigned int texture)
{
    m_openGL_texture = texture;
}

unsigned int Texture::get_OpenGL_Texture()
{
    return m_openGL_texture;
}
