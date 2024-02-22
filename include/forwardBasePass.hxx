//
// Created by Eugene Karpenko @ CaptainGPU
// https://twitter.com/CaptainGPU
//

#pragma once

class RenderPass;
class RenderInfo;
class FrameBuffer;

RenderPass* registerForwardBasePass();

void renderForwardBasePass(unsigned int width, unsigned int height, RenderPass* renderPass, RenderInfo& renderInfo);

FrameBuffer* getForwardBasePassFrameBuffer();