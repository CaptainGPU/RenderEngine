//
// Created by Eugene Karpenko @ CaptainGPU
// https://twitter.com/CaptainGPU
//

#include <glad/gl.h>

class PassProgramm
{
public:
	PassProgramm();

	void set_OpenGL_Programm(GLuint programm);
	GLuint get_OpenGL_Programm();

private:
	GLuint m_programm;
};