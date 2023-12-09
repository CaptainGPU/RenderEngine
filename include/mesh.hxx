//
// Created by Eugene Karpenko @ CaptainGPU
// https://twitter.com/CaptainGPU
//
#pragma once

#include "vao.hxx"
#include <glad/gl.h>

class Mesh
{
public:
	Mesh();
	VertexAttributeObject* getVAO();
	void setVAO(VertexAttributeObject* vao);
	void finish();
	
	// OpenGL Staff
	
	GLuint get_OpenGL_VBO();
	void set_OpenGL_VBO(GLuint vbo);
private:
	VertexAttributeObject* m_vao;

	// OpenGL Staff

	GLuint m_vbo;
};