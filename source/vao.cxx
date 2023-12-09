//
// Created by Eugene Karpenko @ CaptainGPU
// https://twitter.com/CaptainGPU
//

#include <vao.hxx>

#include <render.hxx>

VertexAttributeObject::VertexAttributeObject()
{
	m_OGL_vao = 0;
}

void VertexAttributeObject::setOpenGLVAO(GLuint vao)
{
	m_OGL_vao = vao;
}

GLuint VertexAttributeObject::getOpenGLVAO()
{
	return m_OGL_vao;
}

void VertexAttributeObject::finish()
{
	Render::deleteVAO(this);
}

void VertexAttributeObject::init()
{
	Render::createVAO(this);
}