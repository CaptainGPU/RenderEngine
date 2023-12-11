//
// Created by Eugene Karpenko @ CaptainGPU
// https://twitter.com/CaptainGPU
//
#pragma once

#include <cstdint>

struct RenderInfo
{
    uint32_t numRenderObject;
    uint32_t numVertex;
    uint32_t numIndices;
    uint32_t numRenderPasses;
};

void clearRenderInfo(RenderInfo& ri);

void addVertex(RenderInfo& ri, uint32_t num);
void addIndices(RenderInfo& ri, uint32_t num);
void addRenderPass(RenderInfo& ri);
void addRenderObject(RenderInfo& ri);

void showStats(RenderInfo& ri);
