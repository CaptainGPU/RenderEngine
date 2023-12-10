//
// Created by Eugene Karpenko @ CaptainGPU
// https://twitter.com/CaptainGPU
//

#pragma once

#include "defines.hxx"

#if (CURRENT_PLATFORM == PLATFORM_WIN) || (CURRENT_PLATFORM == PLATFORM_MAC)
#include "glad/gl.h"
#endif

#if CURRENT_PLATFORM == PLATFORM_EMSCRIPTEN
#include <GLES3/gl3.h>
#endif

class ElementBufferObject
{
public:
    ElementBufferObject(uint32_t numIndices);
    
    void init();
    uint32_t getNumIndices();
    
    void set_OpenGL_EBO(GLuint ebo);
    GLuint get_OpenGL_EBO();
    
private:
    GLuint m_ebo;
    uint32_t m_numIndices;
};
