//
// Created by Eugene Karpenko @ CaptainGPU
// https://twitter.com/CaptainGPU
//

#include "vertexShader.hxx"

VertexShader::VertexShader()
:m_shader(0)
{

}

void VertexShader::set_OpenGL_Shader(GLuint shader)
{
	m_shader = shader;
}

GLuint VertexShader::get_OpenGL_Shader()
{
	return m_shader;
}
