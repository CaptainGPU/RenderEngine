//
// Created by Eugene Karpenko @ CaptainGPU
// https://twitter.com/CaptainGPU
//

#pragma once

#include <vector>

class RenderPass;
class RenderInfo;
class Texture;

struct PointLightData;

RenderPass* registerPointLightShadowPass();
void renderPointLightShadowsPass(RenderPass* renderPass, RenderInfo& renderInfo, std::vector<PointLightData>& points, int shadowCount);

std::vector<Texture*> getPointLightShadowMapTexture();
std::vector<Texture*> getPointLightShadowMapColorTexture();
