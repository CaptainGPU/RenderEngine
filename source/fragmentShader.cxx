//
// Created by Eugene Karpenko @ CaptainGPU
// https://twitter.com/CaptainGPU
//

#include "fragmentShader.hxx"

FragmentShader::FragmentShader()
:m_shader(0)
{
	
}

void FragmentShader::set_OpenGL_Shader(GLuint shader)
{
	m_shader = shader;
}

GLuint FragmentShader::get_OpenGL_Shader()
{
	return m_shader;
}
