//
// Created by Eugene Karpenko @ CaptainGPU
// https://twitter.com/CaptainGPU
//

#pragma once

#include <string>

#include "vertexShader.hxx"
#include "fragmentShader.hxx"

void loadVertexShader(VertexShader* shader, std::string name);
void loadFragmentShader(FragmentShader* shader, std::string name);