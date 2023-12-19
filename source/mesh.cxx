//
// Created by Eugene Karpenko @ CaptainGPU
// https://twitter.com/CaptainGPU
//
#include "mesh.hxx"
#include "render.hxx"

Mesh::Mesh():
m_vao(nullptr),
m_vbo(0),
m_numVertex(0)
{
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

void Mesh::setNumVertex(uint32_t numVertex)
{
    m_numVertex = numVertex;
}

uint32_t Mesh::getNumVertex()
{
    return m_numVertex;
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

void Mesh::setMeshBound(MeshBound* bound)
{
	m_meshBound = bound;
}

MeshBound* Mesh::getMeshBound()
{
	return m_meshBound;
}

MeshBound::MeshBound():
m_minX(.0f),
m_minY(.0f),
m_minZ(.0f),
m_maxX(.0f),
m_maxY(.0f),
m_maxZ(.0f)
{
}

void MeshBound::updateBound(Vertex& vertex)
{
	// for x
	if (vertex.pos.x < m_minX)
	{
		m_minX = vertex.pos.x;
	}

	if (vertex.pos.x > m_maxX)
	{
		m_maxX = vertex.pos.x;
	}

	// for y
	if (vertex.pos.y < m_minY)
	{
		m_minY = vertex.pos.y;
	}

	if (vertex.pos.y > m_maxY)
	{
		m_maxY = vertex.pos.y;
	}

	// for z
	if (vertex.pos.z < m_minZ)
	{
		m_minZ = vertex.pos.z;
	}

	if (vertex.pos.z > m_maxZ)
	{
		m_maxZ = vertex.pos.z;
	}
}

std::vector<Vertex> MeshBound::getBoundVertex()
{
	Vertex A, B, C, D, E, F, G, H;

	A.pos = { m_maxX, m_maxY, m_maxZ };
	B.pos = { m_maxX, m_minY, m_maxZ };
	C.pos = { m_minX, m_minY, m_maxZ };
	D.pos = { m_minX, m_maxY, m_maxZ };

	E.pos = { m_maxX, m_maxY, m_minZ };
	F.pos = { m_maxX, m_minY, m_minZ };
	G.pos = { m_minX, m_minY, m_minZ };
	H.pos = { m_minX, m_maxY, m_minZ };

	return std::vector<Vertex> {A, B, C, D, E, F, G, H};
}

VertexAttributeObject* MeshBound::getVAO()
{
	return m_vao;
}

void MeshBound::setVAO(VertexAttributeObject* vao)
{
	m_vao = vao;
}

ElementBufferObject* MeshBound::getEBO()
{
	return m_ebo;
}

void MeshBound::setEBO(ElementBufferObject* ebo)
{
	m_ebo = ebo;
}

GLuint MeshBound::get_OpenGL_VBO()
{
	return m_vbo;
}

void MeshBound::set_OpenGL_VBO(GLuint vbo)
{
	m_vbo = vbo;
}
