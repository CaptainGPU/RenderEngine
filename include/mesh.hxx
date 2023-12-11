//
// Created by Eugene Karpenko @ CaptainGPU
// https://twitter.com/CaptainGPU
//
#pragma once

#include "vao.hxx"
#include "ebo.hxx"

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
    
private:
	VertexAttributeObject* m_vao;
    ElementBufferObject* m_ebo;
    
    uint32_t m_numVertex;

	// OpenGL Staff

	GLuint m_vbo;
};
