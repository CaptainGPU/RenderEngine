//
// Created by Eugene Karpenko @ CaptainGPU
// https://twitter.com/CaptainGPU
//

#pragma once

class RenderPass;
class RenderInfo;
class FrameBuffer;

struct AORenderContext
{
	float radius;
	float bias;
	int kernels;
	int blurSamples;
};

void initMobileSSAOPassData();

RenderPass* registerMobileSSAOORenderPass();

void renderMobileSSAOPass(RenderPass* renderPass, RenderInfo& renderInfo, AORenderContext& context);

FrameBuffer* getDepthPrePassFrameBuffer();
