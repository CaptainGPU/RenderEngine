//
// Created by Eugene Karpenko @ CaptainGPU
// https://twitter.com/CaptainGPU
//

#pragma once

class RenderPass;
class RenderInfo;
class FrameBuffer;

void initMobileSSAOPassData();

RenderPass* registerMobileSSAOORenderPass();

void renderMobileSSAOPass(RenderPass* renderPass, RenderInfo& renderInfo);

FrameBuffer* getDepthPrePassFrameBuffer();