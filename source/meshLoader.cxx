//
// Created by Eugene Karpenko @ CaptainGPU
// https://twitter.com/CaptainGPU
//

#include "meshLoader.hxx"

#include "render.hxx"

#include <tiny_obj_loader.h>
#include <vector>
#include <string>
#include <iostream>
#include <map>

static std::map<std::string, Mesh*> meshMap;

void generateStaticMeshBound(MeshBound* bound)
{
	std::vector<Vertex> vertices = bound->getBoundVertex();
	std::vector<uint32_t> indices = { 
		0, 2, 1, 0, 3, 2,
		4, 5, 6, 4, 6, 7,

		4, 0, 1, 4, 1, 5,
		7, 2, 3, 7, 6, 2,

		7, 3, 0, 7, 0, 4,
		5, 1, 2, 5, 2, 6
	};

	VertexAttributeObject* vao = new VertexAttributeObject();
	vao->init();

	ElementBufferObject* ebo = new ElementBufferObject(indices.size());
	ebo->init();

	GLuint vbo1, vao1, ebo1;
	vao1 = vao->getOpenGLVAO();
	ebo1 = ebo->get_OpenGL_EBO();

	Render::bindVAO(vao);

	glGenBuffers(1, &vbo1);

	glBindBuffer(GL_ARRAY_BUFFER, vbo1);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo1);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), &indices[0], GL_STATIC_DRAW);

	int POS_LOC = 0;
	int UV_LOC = 1;
	int NORMAL_LOC = 2;

	size_t NumFloats = 0;


	// XYZ data
	glEnableVertexAttribArray(POS_LOC);
	glVertexAttribPointer(POS_LOC, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)(NumFloats * sizeof(float)));
	NumFloats += 3;

	// UV data
	glEnableVertexAttribArray(UV_LOC);
	glVertexAttribPointer(UV_LOC, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)(NumFloats * sizeof(float)));
	NumFloats += 2;

	// normals
	glEnableVertexAttribArray(NORMAL_LOC);
	glVertexAttribPointer(NORMAL_LOC, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)(NumFloats * sizeof(float)));
	NumFloats += 3;

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	bound->set_OpenGL_VBO(vbo1);
	bound->setVAO(vao);
	bound->setEBO(ebo);
}

Mesh* loadMesh()
{
	std::string modelFilePath = "../Models/crash_normal.mesh";

	if (rand() % 2)
	{
		modelFilePath = "../Models/monkey.mesh";
	}

	Mesh* mesh = nullptr;

	if (meshMap.find(modelFilePath) != meshMap.end())
	{
		mesh = meshMap.at(modelFilePath);
	}

	if (mesh)
	{
		return mesh;
	}

	mesh = new Mesh();
	
	MeshBound* bound = new MeshBound();

	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string warn, err;

	if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, modelFilePath.c_str()))
	{
		printf("failLoad\n");
		return nullptr;
	}

	if (!warn.empty()) {
		std::cout << warn << std::endl;
	}

	if (!err.empty()) {
		std::cerr << err << std::endl;
	}

	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;

	for (size_t s = 0; s < shapes.size(); s++) 
	{
		size_t index_offset = 0;
		for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++)
		{
			size_t fv = size_t(shapes[s].mesh.num_face_vertices[f]);
			for (size_t v = 0; v < fv; v++)
			{
				tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];

				tinyobj::real_t vx = attrib.vertices[3 * size_t(idx.vertex_index) + 0];
				tinyobj::real_t vy = attrib.vertices[3 * size_t(idx.vertex_index) + 1];
				tinyobj::real_t vz = attrib.vertices[3 * size_t(idx.vertex_index) + 2];

				tinyobj::real_t tx = .0f;
				tinyobj::real_t ty = .0f;

				if (idx.texcoord_index >= 0) {
					tx = attrib.texcoords[2 * size_t(idx.texcoord_index) + 0];
					ty = attrib.texcoords[2 * size_t(idx.texcoord_index) + 1];
				}

				Vertex vertex;
				vertex.pos = {
					vx, vy, vz
				};
				vertex.uv = {
					tx, ty
				};

				bound->updateBound(vertex);

				indices.push_back(vertices.size());
				vertices.push_back(vertex);
			}
			index_offset += fv;
		}
	}

	std::string name = "";

	VertexAttributeObject* vao = new VertexAttributeObject();
	vao->init();

	ElementBufferObject* ebo = new ElementBufferObject(indices.size());
	ebo->init();

	GLuint vbo1, vao1, ebo1;
	vao1 = vao->getOpenGLVAO();
	ebo1 = ebo->get_OpenGL_EBO();

	Render::bindVAO(vao);

	glGenBuffers(1, &vbo1);

	glBindBuffer(GL_ARRAY_BUFFER, vbo1);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo1);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), &indices[0], GL_STATIC_DRAW);

	int POS_LOC = 0;
	int UV_LOC = 1;
	int NORMAL_LOC = 2;

	size_t NumFloats = 0;


	// XYZ data
	glEnableVertexAttribArray(POS_LOC);
	glVertexAttribPointer(POS_LOC, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)(NumFloats * sizeof(float)));
	NumFloats += 3;

	// UV data
	glEnableVertexAttribArray(UV_LOC);
	glVertexAttribPointer(UV_LOC, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)(NumFloats * sizeof(float)));
	NumFloats += 2;

	// normals
	glEnableVertexAttribArray(NORMAL_LOC);
	glVertexAttribPointer(NORMAL_LOC, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)(NumFloats * sizeof(float)));
	NumFloats += 3;

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	generateStaticMeshBound(bound);

	mesh->set_OpenGL_VBO(vbo1);
	mesh->setVAO(vao);
	mesh->setEBO(ebo);
	mesh->setNumVertex(vertices.size());
	mesh->setMeshBound(bound);

	meshMap.insert(std::pair<std::string, Mesh*>(modelFilePath, mesh));

	return mesh;
}
