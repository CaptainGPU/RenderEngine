//
// Created by Eugene Karpenko @ CaptainGPU
// https://twitter.com/CaptainGPU
//

#pragma once

#include "render.hxx"

#include <glm/glm.hpp>

struct Vertex
{
	glm::vec3 pos;
	glm::vec2 uv;
	glm::vec3 normal;
};

Mesh* loadMesh();