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
    void setVec4(glm::vec4& value);
    void setMatrix4x4(glm::mat4& matrix);
    void setFloat(float& value);
    void setFloatArray(float* array, uint32_t size);
    void setInt(int& value);
    void setTexture(Texture* texture, unsigned int slot);
    void setCubeTexture(Texture* texture, unsigned int slot);
    
private:
    GLint m_ogl_uniformID;
};
