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

#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "tiny_gltf.hxx"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <random>

#include "gameObject.hxx"
#include "scene.hxx"

static std::map<std::string, Mesh*> meshMap;
static std::map< std::string, std::vector<Mesh*>> meshesMap;

void generateStaticMeshBound(MeshBound* bound)
{
	std::vector<Vertex> vertices = bound->getBoundVertex();
	std::vector<uint32_t> indices = { 
		0, 1, 0, 3, 3, 2, 2, 1,

		4, 5, 5, 6, 6, 7, 7, 4,

		3, 7, 0, 4, 2, 6, 1, 5
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

Mesh* loadMesh(std::string modelName)
{
    std::string modelFolder = "../Models/";
    
#if CURRENT_PLATFORM == PLATFORM_MAC
    modelFolder = "../../Models/";
#endif
    
	std::string modelFilePath = modelFolder + modelName;

//	if (rand() % 2)
//	{
//		modelFilePath = "../../Models/monkey.mesh";
//	}

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
		printf("failLoad: %s\n", modelFilePath.c_str());
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
                
                tinyobj::real_t nx = .0f;
                tinyobj::real_t ny = .0f;
                tinyobj::real_t nz = .0f;

				if (idx.texcoord_index >= 0) {
					tx = attrib.texcoords[2 * size_t(idx.texcoord_index) + 0];
					ty = attrib.texcoords[2 * size_t(idx.texcoord_index) + 1];
				}
                
                if (idx.normal_index >= 0)
                {
                    nx = attrib.normals[3 * idx.normal_index + 0];
                    ny = attrib.normals[3 * idx.normal_index + 1];
                    nz = attrib.normals[3 * idx.normal_index + 2];
                }

				Vertex vertex;
				vertex.pos = {
					vx, vy, vz
				};
				vertex.uv = {
					tx, ty
				};
                vertex.normal = {
                    nx, ny, nz
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

MeshBound* createCorterBound(std::vector<glm::vec4>& corners)
{
	//std::vector<glm::vec4> corners = getCorners(proj, view);
	std::vector<Vertex> vertices;

	for (const auto& v : corners)
	{
		Vertex vert = { glm::vec3(0.0f), glm::vec2(0.0f), glm::vec3(0.0f) };
		vert.pos = glm::vec3(v);
		vertices.push_back(vert);
	}

	std::vector<uint32_t> indices = {
		0, 1,
		1, 5,
		5, 4,
		4, 0,
		2, 3,
		3, 7,
		7, 6,
		6, 2,
		0, 2,
		3, 1,
		7, 5,
		6, 4
	};

	MeshBound* bound = new MeshBound();

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

	bound->set_OpenGL_VBO(vbo1);
	bound->setVAO(vao);
	bound->setEBO(ebo);

	return bound;
}

Mesh* registerMesh(std::string modelFilePath, std::vector<Vertex>& vertices, std::vector<uint32_t>& indices)
{
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
	int COLOR_LOC = 3;

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

	// colors
	glEnableVertexAttribArray(COLOR_LOC);
	glVertexAttribPointer(COLOR_LOC, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)(NumFloats * sizeof(float)));
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

std::vector<Mesh*> loadMeshes(std::string modelName)
{
	std::string modelFolder = "../Models/";

#if CURRENT_PLATFORM == PLATFORM_MAC
	modelFolder = "../../Models/";
#endif

	std::string modelFilePath = modelFolder + modelName;

	std::vector<Mesh*> subMeshes;

	if (meshesMap.find(modelFilePath) != meshesMap.end())
	{
		subMeshes = meshesMap.at(modelFilePath);
	}

	if (meshesMap.size() > 0)
	{
		return subMeshes;
	}

	tinyobj::ObjReaderConfig reader_config;
	reader_config.mtl_search_path = modelFolder;

	tinyobj::ObjReader reader;

	if (!reader.ParseFromFile(modelFilePath, reader_config)) {
		if (!reader.Error().empty()) {
			std::cerr << "TinyObjReader: " << reader.Error();
		}
		exit(1);
	}

	if (!reader.Warning().empty()) {
		std::cout << "TinyObjReader: " << reader.Warning();
	}

	auto& attrib = reader.GetAttrib();
	auto& shapes = reader.GetShapes();
	auto& materials = reader.GetMaterials();

	std::vector<std::vector<Vertex>> subVertices;
	std::vector<std::vector<uint32_t>> subIndices;

	subVertices.resize(materials.size());
	subIndices.resize(materials.size());

	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;

	for (size_t s = 0; s < shapes.size(); s++) {
		// Loop over faces(polygon)
		size_t index_offset = 0;
		for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
			size_t fv = size_t(shapes[s].mesh.num_face_vertices[f]);

			int materialID = shapes[s].mesh.material_ids[f];

			{

				// Loop over vertices in the face.
				for (size_t v = 0; v < fv; v++) {
					// access to vertex
					Vertex vert;

					tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
					tinyobj::real_t vx = attrib.vertices[3 * size_t(idx.vertex_index) + 0];
					tinyobj::real_t vy = attrib.vertices[3 * size_t(idx.vertex_index) + 1];
					tinyobj::real_t vz = attrib.vertices[3 * size_t(idx.vertex_index) + 2];

					vert.pos = glm::vec3(vx, vy, vz);

					// Check if `normal_index` is zero or positive. negative = no normal data
					if (idx.normal_index >= 0) {
						tinyobj::real_t nx = attrib.normals[3 * size_t(idx.normal_index) + 0];
						tinyobj::real_t ny = attrib.normals[3 * size_t(idx.normal_index) + 1];
						tinyobj::real_t nz = attrib.normals[3 * size_t(idx.normal_index) + 2];
						vert.normal = glm::vec3(nx, ny, nz);
					}

					// Check if `texcoord_index` is zero or positive. negative = no texcoord data
					if (idx.texcoord_index >= 0) {
						tinyobj::real_t tx = attrib.texcoords[2 * size_t(idx.texcoord_index) + 0];
						tinyobj::real_t ty = attrib.texcoords[2 * size_t(idx.texcoord_index) + 1];
						vert.uv = glm::vec2(tx, ty);
					}

					subIndices[materialID].push_back(subVertices[materialID].size());
					subVertices[materialID].push_back(vert);

					// Optional: vertex colors
					// tinyobj::real_t red   = attrib.colors[3*size_t(idx.vertex_index)+0];
					// tinyobj::real_t green = attrib.colors[3*size_t(idx.vertex_index)+1];
					// tinyobj::real_t blue  = attrib.colors[3*size_t(idx.vertex_index)+2];
				}
			}


			index_offset += fv;

			// per-face material
			//shapes[s].mesh.material_ids[f];
		}
	}

	subMeshes.resize(materials.size());

	for (size_t i = 0; i < materials.size(); i++)
	{
		Material* material = new Material();

		material->color = glm::vec3(materials[i].diffuse[0], materials[i].diffuse[1], materials[i].diffuse[2]);

		Mesh* mesh = registerMesh(modelFilePath + std::string("_sub_") + std::to_string(i), subVertices[i], subIndices[i]);
		mesh->setMaterial(material);
		subMeshes[i] = mesh;
	}

	meshesMap.insert(std::pair<std::string, std::vector<Mesh*>>(modelFilePath, subMeshes));

	return subMeshes;
}

Mesh* parseGLTFMesh(const tinygltf::Model& model, const tinygltf::Mesh& mesh, const std::string& modelFilePath)
{
	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;

	MeshBound* bound = new MeshBound();

	std::random_device rd;
	std::mt19937 gen(rd());

	std::uniform_real_distribution<float> dis(0.0f, 1.0f);

	glm::vec3 meshColor = glm::vec3(dis(gen), dis(gen), dis(gen));

	for (size_t j = 0; j < mesh.primitives.size(); j++)
	{

		const tinygltf::Primitive& primitive = mesh.primitives[j];
		assert(primitive.attributes.find("POSITION") != primitive.attributes.end());

		const tinygltf::Accessor& posAccessor = model.accessors[primitive.attributes.find("POSITION")->second];
		const tinygltf::BufferView& posView = model.bufferViews[posAccessor.bufferView];
		const float* bufferPosbufferPos = reinterpret_cast<const float*>(&(model.buffers[posView.buffer].data[posAccessor.byteOffset + posView.byteOffset]));
		glm::vec3 posMin = glm::vec3(posAccessor.minValues[0], posAccessor.minValues[1], posAccessor.minValues[2]);
		glm::vec3 posMax = glm::vec3(posAccessor.maxValues[0], posAccessor.maxValues[1], posAccessor.maxValues[2]);
		uint32_t vertexCount = static_cast<uint32_t>(posAccessor.count);
		int posByteStride = posAccessor.ByteStride(posView) ? (posAccessor.ByteStride(posView) / sizeof(float)) : tinygltf::GetNumComponentsInType(TINYGLTF_TYPE_VEC3);

		int normByteStride;

		const float* bufferNormals = nullptr;
		if (primitive.attributes.find("NORMAL") != primitive.attributes.end()) {
			const tinygltf::Accessor& normAccessor = model.accessors[primitive.attributes.find("NORMAL")->second];
			const tinygltf::BufferView& normView = model.bufferViews[normAccessor.bufferView];
			bufferNormals = reinterpret_cast<const float*>(&(model.buffers[normView.buffer].data[normAccessor.byteOffset + normView.byteOffset]));
			normByteStride = normAccessor.ByteStride(normView) ? (normAccessor.ByteStride(normView) / sizeof(float)) : tinygltf::GetNumComponentsInType(TINYGLTF_TYPE_VEC3);
		}

		const float* bufferPos = reinterpret_cast<const float*>(&(model.buffers[posView.buffer].data[posAccessor.byteOffset + posView.byteOffset]));

		for (size_t v = 0; v < posAccessor.count; v++)
		{
			Vertex vert{};
			vert.pos = glm::vec3(glm::make_vec3(&bufferPos[v * posByteStride]));

			vert.normal = glm::normalize(glm::vec3(bufferNormals ? glm::make_vec3(&bufferNormals[v * normByteStride]) : glm::vec3(0.0f)));

			vert.color = meshColor;

			vertices.push_back(vert);
			bound->updateBound(vert);
		}

		bool hasIndices = primitive.indices > -1;

		if (hasIndices)
		{
			const tinygltf::Accessor& accessor = model.accessors[primitive.indices > -1 ? primitive.indices : 0];
			const tinygltf::BufferView& bufferView = model.bufferViews[accessor.bufferView];
			const tinygltf::Buffer& buffer = model.buffers[bufferView.buffer];

			const void* dataPtr = &(buffer.data[accessor.byteOffset + bufferView.byteOffset]);

			switch (accessor.componentType)
			{
			case TINYGLTF_PARAMETER_TYPE_UNSIGNED_INT: {
				const uint32_t* buf = static_cast<const uint32_t*>(dataPtr);
				for (size_t index = 0; index < accessor.count; index++) {
					uint32_t ind = buf[index];
					indices.push_back(ind);
				}
				break;
			}

			case TINYGLTF_PARAMETER_TYPE_UNSIGNED_SHORT: {
				const uint16_t* buf = static_cast<const uint16_t*>(dataPtr);
				for (size_t index = 0; index < accessor.count; index++) {
					uint32_t ind = buf[index];
					indices.push_back(ind);
				}
				break;
			}

			case TINYGLTF_PARAMETER_TYPE_UNSIGNED_BYTE: {
				const uint8_t* buf = static_cast<const uint8_t*>(dataPtr);
				for (size_t index = 0; index < accessor.count; index++) {
					uint32_t ind = buf[index];
					indices.push_back(ind);
				}
				break;
			}

			default:
				return nullptr;
			}
		}

		int a = 10;
	}

	generateStaticMeshBound(bound);

	Mesh* meshModel = registerMesh(modelFilePath, vertices, indices);
	meshModel->setMaterial(new Material());
	meshModel->setMeshBound(bound);

	return meshModel;
}

void parseGameObject(GameObject* parent, glm::mat4 parentMatrix, const tinygltf::Model& model, const tinygltf::Node& node, std::vector<Mesh*>& subMeshes, Scene* scene)
{
	//if (node.mesh > -1)
	{


		GameObject* gameObject = new GameObject(node.name);

		glm::vec3 translation = glm::vec3(0.0f);
		if (node.translation.size() == 3)
		{
			translation = glm::make_vec3(node.translation.data());
			gameObject->setPosition(translation);
		}

		if (node.rotation.size() == 4)
		{
			glm::quat q = glm::make_quat(node.rotation.data());
			glm::vec3 rotation = glm::eulerAngles(q);
			rotation = (rotation * 180.0f) / glm::pi<float>();
			gameObject->SetRotateX(rotation.x);
			gameObject->SetRotateY(rotation.y);
			gameObject->SetRotateZ(rotation.z);
		}

		glm::vec3 scale = glm::vec3(1.0f);
		if (node.scale.size() == 3)
		{
			scale = glm::make_vec3(node.scale.data());
			gameObject->setScale(scale);
		}

		glm::mat4 nodeMatrix = glm::mat4(1.0f);
		if (node.matrix.size() == 16)
		{
			nodeMatrix = glm::make_mat4x4(node.matrix.data());
		}


		glm::mat4 localMatrix = gameObject->getModelMatrix();
		localMatrix = nodeMatrix* localMatrix;
		
		glm::mat4 worldMatrix = parentMatrix * localMatrix;
		glm::vec3 worldPosition = worldMatrix[3];
		glm::vec3 worldScale = glm::vec3(.0);
		glm::quat rotation;
		glm::vec3 worldRotation = glm::vec3(0.0);
		for (int i = 0; i < 3; i++)
			worldScale[i] = glm::length(glm::vec3(worldMatrix[i]));

		const glm::mat3 rotMtx(
			glm::vec3(worldMatrix[0]) / worldScale[0],
			glm::vec3(worldMatrix[1]) / worldScale[1],
			glm::vec3(worldMatrix[2]) / worldScale[2]);

		rotation = glm::quat_cast(rotMtx);
		worldRotation = glm::eulerAngles(rotation);
		worldRotation = (worldRotation * 180.0f) / glm::pi<float>();
		
		gameObject->setPosition(worldPosition);
		gameObject->setScale(worldScale);
		gameObject->setRotation(worldRotation);

		
		if (node.mesh > -1)
		{
			gameObject->addMesh(subMeshes[node.mesh]);
		}

		if (gameObject)
		{
			scene->addGameObject(gameObject);
		}

		for (size_t j = 0; j < node.children.size(); j++)
		{
			const tinygltf::Node& child = model.nodes[node.children[j]];
			parseGameObject(gameObject, worldMatrix, model, child, subMeshes, scene);
		}
	}
}

void loadGLTFtoScene(Scene* wscene, const std::string& modelName)
{
	std::string modelFolder = "../Models/";

#if CURRENT_PLATFORM == PLATFORM_MAC
	modelFolder = "../../Models/";
#endif

	std::string modelFilePath = modelFolder + modelName;

	std::vector<Mesh*> subMeshes;

	float scale = 1.0;
	tinygltf::Model model;
	tinygltf::TinyGLTF loader;
	std::string err;
	std::string warn;

	std::vector<Material> materials;
	materials.push_back(Material());

	size_t indexCount = 0;

	bool ret = loader.LoadASCIIFromFile(&model, &err, &warn, modelFilePath);

	if (!warn.empty()) {
		printf("Warn: %s\n", warn.c_str());
	}

	if (!err.empty()) {
		printf("Err: %s\n", err.c_str());
	}

	if (!ret) {
		printf("Failed to parse glTF\n");
	}

	size_t vertexPos = 0;
	size_t indexPos = 0;

	if (ret)
	{
		for (size_t i = 0; i < model.meshes.size(); i++)
		{
			const tinygltf::Mesh mesh = model.meshes[i];

			Mesh* meshModel = parseGLTFMesh(model, mesh, modelFilePath + "_" + std::to_string(i));

			subMeshes.push_back(meshModel);
		}

		// load gameObjects

		const tinygltf::Scene& scene = model.scenes[model.defaultScene > -1 ? model.defaultScene : 0];

		glm::mat4 parentMatrix = glm::mat4(1.0);
		
		for (size_t i = 0; i < scene.nodes.size(); i++)
		{
			const tinygltf::Node node = model.nodes[scene.nodes[i]];

			parseGameObject(nullptr, parentMatrix, model, node, subMeshes, wscene);
		}

	}

	//return subMeshes;
}


void addQuad(float x, float y, std::vector<Vertex>& vertices, std::vector<uint32_t>& indices)
{
    size_t count = vertices.size();
    
    Vertex a = Vertex();
    Vertex b = Vertex();
    Vertex c = Vertex();
    Vertex d = Vertex();
    
    a.pos = glm::vec3(0.0, 0.0, 0.0);
    a.color = glm::vec3(0.0, 0.0, 0.0);
    
    b.pos = glm::vec3( 1.0, 0.0, 0.0);
    b.color = glm::vec3(1.0, 0.0, 0.0);
    
    c.pos = glm::vec3(0.0, 1.0, 0.0);
    c.color = glm::vec3(0.0, 1.0, 0.0);
    
    d.pos = glm::vec3(1.0, 1.0, 0.0);
    d.color = glm::vec3(1.0, 1.0, 0.0);
    
    a.pos.x += x;
    b.pos.x += x;
    c.pos.x += x;
    d.pos.x += x;
    
    a.pos.y += y;
    b.pos.y += y;
    c.pos.y += y;
    d.pos.y += y;
    
    vertices.push_back(a);
    vertices.push_back(b);
    vertices.push_back(c);
    vertices.push_back(d);
    
    std::vector<uint32_t> newIndices;
    newIndices.reserve(6);
    
    newIndices.push_back(0 + count);
    newIndices.push_back(1 + count);
    newIndices.push_back(2 + count);
    newIndices.push_back(1 + count);
    newIndices.push_back(3 + count);
    newIndices.push_back(2 + count);
    
    indices.insert(indices.end(), newIndices.begin(), newIndices.end());
}

Mesh* createQuad()
{
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
    
    for (size_t i = 0; i < 25; i++)
    {
        for (size_t j = 0; j < 19; j++)
        {
            addQuad((float)i, float(j), vertices, indices);
        }
    }
    
    Mesh* mesh = registerMesh("create_quad", vertices, indices);
    
    return mesh;
}
