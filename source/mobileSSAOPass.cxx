//
// Created by Eugene Karpenko @ CaptainGPU
// https://twitter.com/CaptainGPU
//

#include "mobileSSAOPass.hxx"

#include "renderPass.hxx"
#include "screenRenderPass.hxx"
#include "render.hxx"
#include "engine.hxx"

#define HALF_PI (1.57079632679f)

#include <vector>
#include <algorithm>
#include <random>

namespace MobileSSAOPass
{
	RenderPass* depthPrePass = nullptr;
	FrameBuffer* depthPrePassFrameBuffer = nullptr;
	Uniform* depthPrePassModelMatrixUniform = nullptr;
	Uniform* depthPrePassViewMatrixUniform = nullptr;
	Uniform* depthPrePassProjectionMatrixUniform = nullptr;

	Texture* GTAOPreIntegratedTexture = nullptr;

	ScreenRenderPass* horizonSerchIntegralPass = nullptr;
	FrameBuffer* HSIPassFrameBuffer = nullptr;
	Uniform* HSI_worldPositionTextureUniform = nullptr;
	Uniform* HSI_worldNormalTextureUniform = nullptr;
	std::vector<glm::vec3> HSI_kernel;
	std::vector<glm::vec3> HSI_noise;
	Texture* HSI_noiseTexture = nullptr;
	Uniform* HSI_noiseTextureUniform = nullptr;
	Uniform* HSI_projectionMatrixUniform = nullptr;
	Uniform* HSI_AORadiusUniform = nullptr;
	Uniform* HSI_AOBiasUniform = nullptr;
	Uniform* HSI_AOKernelsUniform = nullptr;
	std::vector<Uniform*> HSI_samplesUniform;
}

glm::vec4 CreateInvDeviceZToWorldZTransform(glm::mat4& projMatrix)
{
	float DepthMul = (float)projMatrix[2][2];
	float DepthAdd = (float)projMatrix[3][2];

	if (DepthAdd == 0.f)
	{
		// Avoid dividing by 0 in this case
		DepthAdd = 0.00000001f;
	}

	bool bIsPerspectiveProjection = projMatrix[3][3] < 1.0f;

	if (bIsPerspectiveProjection)
	{
		float SubtractValue = DepthMul / DepthAdd;

		SubtractValue -= 0.00000001f;

		return glm::vec4(
			0.0f,
			0.0f,
			1.0f / DepthAdd,
			SubtractValue
		);
	}
	else
	{
		return glm::vec4(
			(float)(1.0f / projMatrix[2][2]),
			(float)(-projMatrix[3][2] / projMatrix[2][2] + 1.0f),
			0.0f,
			1.0f
		);
	}
}

float ourLerp(float a, float b, float f)
{
	return a + f * (b - a);
}

void initMobileSSAOPassData()
{
	size_t extend = 16;
	size_t square = extend * extend;

	std::vector<float> tempBuffer(extend * extend * extend);

	for (size_t i = 0; i < tempBuffer.size(); i++)
	{
		tempBuffer[i] = 0.0f;
	}

	float* destBuffer = tempBuffer.data();

	for (size_t z = 0; z < extend; ++z)
	{
		for (size_t y = 0; y < extend; ++y)
		{
			for (size_t x = 0; x < extend; ++x)
			{
				unsigned int destBufferIndex = x + y * extend + z * square;

				float value = destBuffer[destBufferIndex];

				float cosAngle1 = ((x + 0.5f) / (extend)-0.5f) * 2;
				float cosAngle2 = ((y + 0.5f) / (extend)-0.5f) * 2;
				float cosAng = ((z + 0.5f) / (extend)-0.5f) * 2;

				float Gamma = acos(cosAng) - HALF_PI;
				float CosGamma = cos(Gamma);
				float SinGamma = cosAng * -2.0f;

				float Angle1 = acos(cosAngle1);
				float Angle2 = acos(cosAngle2);

				// clamp to normal hemisphere 
				Angle1 = Gamma + std::max(-Angle1 - Gamma, -(HALF_PI));
				Angle2 = Gamma + std::min(Angle2 - Gamma, (HALF_PI));

				float AO = (0.25f *
					((Angle1 * SinGamma + CosGamma - cos((2.0 * Angle1) - Gamma)) +
						(Angle2 * SinGamma + CosGamma - cos((2.0 * Angle2) - Gamma))));

				tempBuffer[destBufferIndex] = AO;
			}
		}
	}

	MobileSSAOPass::GTAOPreIntegratedTexture = Render::make3DTextureFloat(tempBuffer, extend, extend, extend);

	std::uniform_real_distribution<float> randomFloats(0.0, 1.0);
	std::default_random_engine generator;

	for (size_t i = 0; i < 64; ++i)
	{
		glm::vec3 sample(randomFloats(generator) * 2.0 - 1.0, randomFloats(generator) * 2.0 - 1.0, randomFloats(generator));
		sample = glm::normalize(sample);
		sample *= randomFloats(generator);
		float scale = float(i) / 64.0f;

		// scale samples s.t. they're more aligned to center of kernel
		scale = ourLerp(0.1f, 1.0f, scale * scale);
		sample *= scale;
		MobileSSAOPass::HSI_kernel.push_back(sample);
	}

	for (unsigned int i = 0; i < 16; i++)
	{
		glm::vec3 noise(randomFloats(generator) * 2.0 - 1.0, randomFloats(generator) * 2.0 - 1.0, 0.0f); // rotate around z-axis (in tangent space)
		MobileSSAOPass::HSI_noise.push_back(noise);
	}

	MobileSSAOPass::HSI_noiseTexture = Render::makeNoiseLinearTexture(MobileSSAOPass::HSI_noise);
}

RenderPass* registerMobileSSAOORenderPass()
{
	std::vector<std::string> uniformNames = {};

	RenderPass* renderPass = new RenderPass();
	renderPass->makeProgram("forwardBasePass", "forwardBasePass");
	renderPass->registerUniforms(uniformNames);

	// Depth PrePass

	std::vector<std::string> depthPrePassUniformNames = { "u_modelMatrix", "u_viewMatrix", "u_projectionMatrix" };

	MobileSSAOPass::depthPrePass = new RenderPass();
	MobileSSAOPass::depthPrePass->makeProgram("depthPrePass", "depthPrePass");
	MobileSSAOPass::depthPrePass->registerUniforms(depthPrePassUniformNames);

	MobileSSAOPass::depthPrePassModelMatrixUniform = MobileSSAOPass::depthPrePass->getUniform(depthPrePassUniformNames[0]);
	MobileSSAOPass::depthPrePassViewMatrixUniform = MobileSSAOPass::depthPrePass->getUniform(depthPrePassUniformNames[1]);
	MobileSSAOPass::depthPrePassProjectionMatrixUniform = MobileSSAOPass::depthPrePass->getUniform(depthPrePassUniformNames[2]);

	MobileSSAOPass::depthPrePassFrameBuffer = Render::createCustomFrameBuffer(800, 600);

	// Horison Search Integrall Pass

	std::vector<std::string> HSIPassUniformNames = { "u_texture_0", "u_texture_1", "u_texture_2", "u_projectionMatrix", "u_aoRadius", "u_aoBias", "u_aoKernels"};

	for (size_t i = 0; i < 64; i++)
	{
		char locBuff[100] = { '\0' };

		snprintf(locBuff, sizeof(locBuff), "u_samples[%zu]", i);
		std::string name = std::string(locBuff);
		HSIPassUniformNames.push_back(name);
	}

	MobileSSAOPass::horizonSerchIntegralPass = new ScreenRenderPass();
	MobileSSAOPass::horizonSerchIntegralPass->makeProgram("HSIAOPass", "HSIAOPass");
	MobileSSAOPass::horizonSerchIntegralPass->registerUniforms(HSIPassUniformNames);

	MobileSSAOPass::HSI_worldPositionTextureUniform = MobileSSAOPass::horizonSerchIntegralPass->getUniform(HSIPassUniformNames[0]);
	MobileSSAOPass::HSI_worldNormalTextureUniform = MobileSSAOPass::horizonSerchIntegralPass->getUniform(HSIPassUniformNames[1]);
	MobileSSAOPass::HSI_noiseTextureUniform = MobileSSAOPass::horizonSerchIntegralPass->getUniform(HSIPassUniformNames[2]);
	MobileSSAOPass::HSI_projectionMatrixUniform = MobileSSAOPass::horizonSerchIntegralPass->getUniform(HSIPassUniformNames[3]);
	MobileSSAOPass::HSI_AORadiusUniform = MobileSSAOPass::horizonSerchIntegralPass->getUniform(HSIPassUniformNames[4]);
	MobileSSAOPass::HSI_AOBiasUniform = MobileSSAOPass::horizonSerchIntegralPass->getUniform(HSIPassUniformNames[5]);
	MobileSSAOPass::HSI_AOKernelsUniform = MobileSSAOPass::horizonSerchIntegralPass->getUniform(HSIPassUniformNames[6]);

	for (size_t i = 0; i < 64; i++)
	{
		char locBuff[100] = { '\0' };
		snprintf(locBuff, sizeof(locBuff), "u_samples[%zu]", i);
		std::string name = std::string(locBuff);

		Uniform* uniform = MobileSSAOPass::horizonSerchIntegralPass->getUniform(name);
		MobileSSAOPass::HSI_samplesUniform.push_back(uniform);
	}

	MobileSSAOPass::HSIPassFrameBuffer = Render::createCustomFrameBuffer(800, 600);

	return renderPass;
}

void depthPrePassRender(RenderInfo& renderInfo)
{
	SceneManager* manager = Engine::get()->getSceneManager();
	Scene* scene = manager->getScene();
	Camera* camera = scene->getCamera();

	size_t numGameObject = scene->getGameObjectCount();

	glm::mat4& viewMatrix = camera->getViewMatrix();
	MobileSSAOPass::depthPrePassViewMatrixUniform->setMatrix4x4(viewMatrix);

	glm::mat4& projection_matrix = scene->getCamera()->getProjectionMatrix();
	MobileSSAOPass::depthPrePassProjectionMatrixUniform->setMatrix4x4(projection_matrix);

	Render::activateTexture(MobileSSAOPass::GTAOPreIntegratedTexture, 0);
	Render::bind3DTexture(MobileSSAOPass::GTAOPreIntegratedTexture);

	for (size_t i = 0; i < numGameObject; i++)
	{
		GameObject* gameObject = scene->getGameObject(i);
		unsigned int meshCount = gameObject->getMeshCount();

		if (meshCount == 0 || !gameObject->isRenderingObject())
		{
			continue;
		}

		glm::mat4 modelMatrix = gameObject->getModelMatrix();
		MobileSSAOPass::depthPrePassModelMatrixUniform->setMatrix4x4(modelMatrix);

		for (size_t j = 0; j < meshCount; j++)
		{
			Mesh* mesh = gameObject->getMesh(j);

			Render::drawMesh(mesh, renderInfo);
		}
	}
}

void depthPrePass(RenderInfo& renderInfo)
{
	Render::setViewport(0, 0, 800, 600);
	Render::useFrameBuffer(MobileSSAOPass::depthPrePassFrameBuffer);

	Render::startRenderPass(MobileSSAOPass::depthPrePass, renderInfo);

	Render::clearView(0.0, 0.0, 0.0, 1.0);

	depthPrePassRender(renderInfo);

	Render::endRenderPass(MobileSSAOPass::depthPrePass);

	Render::unUseFrameBuffer();
}

void horisonSearchIntegralRender(RenderInfo& renderInfo, AORenderContext& context)
{
	Texture* worldPos = MobileSSAOPass::depthPrePassFrameBuffer->getColorTexture();
	Texture* worldNormal = MobileSSAOPass::depthPrePassFrameBuffer->getColorTexture1();
	Texture* noise = MobileSSAOPass::HSI_noiseTexture;

	MobileSSAOPass::HSI_worldPositionTextureUniform->setTexture(worldPos, 0);
	MobileSSAOPass::HSI_worldNormalTextureUniform->setTexture(worldNormal, 1);
	MobileSSAOPass::HSI_noiseTextureUniform->setTexture(noise, 2);

	MobileSSAOPass::HSI_AORadiusUniform->setFloat(context.radius);
	MobileSSAOPass::HSI_AOBiasUniform->setFloat(context.bias);
	MobileSSAOPass::HSI_AOKernelsUniform->setInt(context.kernels);

	SceneManager* manager = Engine::get()->getSceneManager();
	Scene* scene = manager->getScene();
	Camera* camera = scene->getCamera();

	for (size_t i = 0; i < MobileSSAOPass::HSI_samplesUniform.size(); i++)
	{
		MobileSSAOPass::HSI_samplesUniform[i]->setVec3(MobileSSAOPass::HSI_kernel[i]);
	}

	glm::mat4 projectionMatrix = camera->getProjectionMatrix();


	MobileSSAOPass::HSI_projectionMatrixUniform->setMatrix4x4(projectionMatrix);


	MobileSSAOPass::horizonSerchIntegralPass->draw(renderInfo);
}

void horisonSearchIntegral(RenderInfo& renderInfo, AORenderContext& context)
{
	Render::setViewport(0, 0, 800, 600);
	Render::useFrameBuffer(MobileSSAOPass::HSIPassFrameBuffer);

	Render::startRenderPass(MobileSSAOPass::horizonSerchIntegralPass, renderInfo);

	Render::clearView(1.0, 1.0, 1.0, 1.0);

	horisonSearchIntegralRender(renderInfo, context);

	Render::endRenderPass(MobileSSAOPass::horizonSerchIntegralPass);

	Render::unUseFrameBuffer();
}

void renderMobileSSAOPass(RenderPass* renderPass, RenderInfo& renderInfo, AORenderContext& context)
{
	depthPrePass(renderInfo);

	horisonSearchIntegral(renderInfo, context);
}

FrameBuffer* getDepthPrePassFrameBuffer()
{
	return MobileSSAOPass::HSIPassFrameBuffer;
}
