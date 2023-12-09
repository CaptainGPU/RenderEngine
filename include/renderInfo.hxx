//
// Created by Eugene Karpenko @ CaptainGPU
// https://twitter.com/CaptainGPU
//
#pragma once

#include <cstdint>

struct RenderInfo
{
    uint32_t numRenderObject;
};

void clearRenderInfo(RenderInfo& ri);