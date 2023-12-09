//
// Created by Eugene Karpenko @ CaptainGPU
// https://twitter.com/CaptainGPU
//

#pragma once

#include <glad/gl.h>

class VertexAttributeObject
{
public:
	VertexAttributeObject();
	virtual void init();
	virtual void finish();

	// OpeGL Staff
	void setOpenGLVAO(GLuint vao);
	GLuint getOpenGLVAO();
private:
	GLuint m_OGL_vao;
};