//
// Created by Eugene Karpenko @ CaptainGPU
// https://twitter.com/CaptainGPU
//

#include <vector>

class RenderPass;
class RenderInfo;
class Texture;

struct SpotLightData;

RenderPass* registerSpotLightShadowPass();

void renderSpotlightShadowsPass(RenderPass* renderPass, RenderInfo& renderInfo, std::vector<SpotLightData>& spots, int shadowCount);

std::vector<Texture*> getSpotLightShadowMapTexture();
std::vector<Texture*> getSpotLightShadowMapColorTexture();
