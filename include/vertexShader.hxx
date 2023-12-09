//
// Created by Eugene Karpenko @ CaptainGPU
// https://twitter.com/CaptainGPU
//

#pragma once

#include <glad/gl.h>

class VertexShader
{
public:
	VertexShader();
	
	void set_OpenGL_Shader(GLuint shader);
	GLuint get_OpenGL_Shader();

private:
	GLuint m_shader;
};