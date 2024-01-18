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

void renderSpotlightShadowsPass(RenderPass* renderPass, RenderInfo& renderInfo, std::vector<SpotLightData>& spots);

std::vector<Texture*> getSpotLightShadowMapTexture();
std::vector<Texture*> getSpotLightShadowMapColorTexture();