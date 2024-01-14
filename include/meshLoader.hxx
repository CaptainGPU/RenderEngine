//
// Created by Eugene Karpenko @ CaptainGPU
// https://twitter.com/CaptainGPU
//

#pragma once

#include "mesh.hxx"

#include <glm/glm.hpp>
#include <string>

Mesh* loadMesh(std::string modelName);
MeshBound* createCorterBound(std::vector<glm::vec4>& corners);
