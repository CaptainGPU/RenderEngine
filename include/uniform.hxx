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

#include <glm/glm.hpp>

class Texture;

class Uniform
{
public:
    Uniform();
    
    void set_OpenGL_uniformID(GLint id);
    GLint get_OpenGL_uniformID();
    
    void setVec3(glm::vec3& value);
    void setMatrix4x4(glm::mat4& matrix);
    void setFloat(float& value);
    void setTexture(Texture* texture);
    
private:
    GLint m_ogl_uniformID;
};
