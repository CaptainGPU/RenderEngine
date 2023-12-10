//
// Created by Eugene Karpenko @ CaptainGPU
// https://twitter.com/CaptainGPU
//
#include "defines.hxx"
#if (CURRENT_PLATFORM == PLATFORM_WIN) || (CURRENT_PLATFORM == PLATFORM_MAC)
#include "glad/gl.h"
#endif

#if CURRENT_PLATFORM == PLATFORM_EMSCRIPTEN
#include <GLES3/gl3.h>
#endif

class FragmentShader
{
public:
	FragmentShader();

	void set_OpenGL_Shader(GLuint shader);
	GLuint get_OpenGL_Shader();

private:
	GLuint m_shader;
};