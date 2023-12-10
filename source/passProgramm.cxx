//
// Created by Eugene Karpenko @ CaptainGPU
// https://twitter.com/CaptainGPU
//

#include "passProgramm.hxx"

PassProgramm::PassProgramm()
:m_programm(0)
{
}

void PassProgramm::set_OpenGL_Programm(GLuint programm)
{
	m_programm = programm;
}

GLuint PassProgramm::get_OpenGL_Programm()
{
	return m_programm;
}

void PassProgramm::finish()
{
    
}
