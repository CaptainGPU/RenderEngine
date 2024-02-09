//
// Created by Eugene Karpenko @ CaptainGPU
// https://twitter.com/CaptainGPU
//

#pragma once

#include "mesh.hxx"

#include <glm/glm.hpp>
#include <string>

class Scene;

Mesh* loadMesh(std::string modelName);
MeshBound* createCorterBound(std::vector<glm::vec4>& corners);

std::vector<Mesh*> loadMeshes(std::string modelName);

void loadGLTFtoScene(Scene* scene, const std::string& modelName);
