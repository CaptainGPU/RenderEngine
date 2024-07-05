//
// Created by Eugene Karpenko @ CaptainGPU
// https://twitter.com/CaptainGPU
//
#pragma once

class RenderPass;
class RenderInfo;
class FrameBuffer;

RenderPass* registerTileMapRenderPass();

void renderTileMapPass(RenderPass* renderPass, RenderInfo& renderInfo);

FrameBuffer* getTileMapPassFrameBuffer();
