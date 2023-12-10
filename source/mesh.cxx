//
// Created by Eugene Karpenko @ CaptainGPU
// https://twitter.com/CaptainGPU
//
#include "mesh.hxx"
#include "render.hxx"

Mesh::Mesh():
m_vao(nullptr),
m_vbo(0)
{
    Render::createVBO(this);
}

VertexAttributeObject* Mesh::getVAO()
{
	return m_vao;
}

void Mesh::setVAO(VertexAttributeObject* vao)
{
	m_vao = vao;
}

ElementBufferObject* Mesh::getEBO()
{
    return m_ebo;
}

void Mesh::setEBO(ElementBufferObject* ebo)
{
    m_ebo = ebo;
}

void Mesh::finish()
{
	if (m_vbo > 0)
	{
		Render::deleteVBO(this);
        m_vbo = 0;
        
        m_ebo = nullptr;
        m_vao = nullptr;
	}
}

GLuint Mesh::get_OpenGL_VBO()
{
	return m_vbo;
}

void Mesh::set_OpenGL_VBO(GLuint vbo)
{
	m_vbo = vbo;
}
