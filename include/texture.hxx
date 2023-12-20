//
// Created by Eugene Karpenko @ CaptainGPU
// https://twitter.com/CaptainGPU
//

#pragma once

class Texture
{
public:
    Texture();
    void set_OpenGL_Texture(unsigned int texture);
    unsigned int get_OpenGL_Texture();
    
private:
    unsigned int m_openGL_texture;
};
