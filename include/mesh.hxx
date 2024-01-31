//
// Created by Eugene Karpenko @ CaptainGPU
// https://twitter.com/CaptainGPU
//
#pragma once

#include "vao.hxx"
#include "ebo.hxx"
#include "material.hxx"

#include <stdint.h>
#include <glm/glm.hpp>
#include <vector>

struct Vertex
{
	glm::vec3 pos;
	glm::vec2 uv;
	glm::vec3 normal;
};

class MeshBound
{
public:
	MeshBound();
	void updateBound(Vertex& vertex);
	std::vector<Vertex> getBoundVertex();

	VertexAttributeObject* getVAO();
	void setVAO(VertexAttributeObject* vao);

	ElementBufferObject* getEBO();
	void setEBO(ElementBufferObject* ebo);

	GLuint get_OpenGL_VBO();
	void set_OpenGL_VBO(GLuint vbo);

private:
	float m_minX;
	float m_minY;
	float m_minZ;

	float m_maxX;
	float m_maxY;
	float m_maxZ;

	VertexAttributeObject* m_vao;
	ElementBufferObject* m_ebo;
	GLuint m_vbo;
};

class Mesh
{
public:
	Mesh();
	VertexAttributeObject* getVAO();
	void setVAO(VertexAttributeObject* vao);
    
    ElementBufferObject* getEBO();
    void setEBO(ElementBufferObject* ebo);
    
    void setNumVertex(uint32_t numVertex);
    uint32_t getNumVertex();
	
    void finish();
	
	// OpenGL Staff
	
	GLuint get_OpenGL_VBO();
	void set_OpenGL_VBO(GLuint vbo);

	void setMeshBound(MeshBound* bound);
	MeshBound* getMeshBound();

	void setMaterial(Material* material);
	Material* getMaterial();
    
private:
	MeshBound* m_meshBound;
	VertexAttributeObject* m_vao;
    ElementBufferObject* m_ebo;
	Material* m_material;
    
    uint32_t m_numVertex;

	// OpenGL Staff

	GLuint m_vbo;
};
