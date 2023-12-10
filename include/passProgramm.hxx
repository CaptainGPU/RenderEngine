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

class PassProgramm
{
public:
	PassProgramm();

	void set_OpenGL_Programm(GLuint programm);
	GLuint get_OpenGL_Programm();

private:
	GLuint m_programm;
};