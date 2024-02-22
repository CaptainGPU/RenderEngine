//
// Created by Eugene Karpenko @ CaptainGPU
// https://twitter.com/CaptainGPU
//

#include "forwardBasePass.hxx"

#include "renderPass.hxx"
#include "render.hxx"
#include "engine.hxx"

#include <vector>
#include <string>

namespace ForwardBasePass
{
	FrameBuffer* forwardBasePassFrameBuffer = nullptr;
	Uniform* modelMatrixUniform = nullptr;
	Uniform* viewMatrixUniform = nullptr;
	Uniform* projectionMatrixUniform = nullptr;
}

RenderPass* registerForwardBasePass()
{
	std::vector<std::string> uniformNames = { "u_modelMatrix", "u_viewMatrix", "u_projectionMatrix" };

	RenderPass* renderPass = new RenderPass();
	renderPass->makeProgram("forwardBasePass", "forwardBasePass");
	renderPass->registerUniforms(uniformNames);

	ForwardBasePass::modelMatrixUniform = renderPass->getUniform(uniformNames[0]);
	ForwardBasePass::viewMatrixUniform = renderPass->getUniform(uniformNames[1]);
	ForwardBasePass::projectionMatrixUniform = renderPass->getUniform(uniformNames[2]);

	ForwardBasePass::forwardBasePassFrameBuffer = Render::createFrameBuffer();

	return renderPass;
}

void basePass(RenderPass* renderPass, RenderInfo& renderInfo)
{
	SceneManager* manager = Engine::get()->getSceneManager();
	Scene* scene = manager->getScene();
	Camera* camera = scene->getCamera();

	size_t numGameObject = scene->getGameObjectCount();

	glm::mat4& viewMatrix = camera->getViewMatrix();
	ForwardBasePass::viewMatrixUniform->setMatrix4x4(viewMatrix);

	glm::mat4& projection_matrix = scene->getCamera()->getProjectionMatrix();
	ForwardBasePass::projectionMatrixUniform->setMatrix4x4(projection_matrix);

	for (size_t i = 0; i < numGameObject; i++)
	{
		GameObject* gameObject = scene->getGameObject(i);
		unsigned int meshCount = gameObject->getMeshCount();

		if (meshCount == 0 || !gameObject->isRenderingObject())
		{
			continue;
		}

		glm::mat4 modelMatrix = gameObject->getModelMatrix();
		ForwardBasePass::modelMatrixUniform->setMatrix4x4(modelMatrix);

		for (size_t j = 0; j < meshCount; j++)
		{
			Mesh* mesh = gameObject->getMesh(j);

			Render::drawMesh(mesh, renderInfo);
		}
	}
}

void renderForwardBasePass(unsigned int width, unsigned int height, RenderPass* renderPass, RenderInfo& renderInfo)
{
	Render::setViewport(0, 0, width, height);
	Render::useFrameBuffer(ForwardBasePass::forwardBasePassFrameBuffer);

	Render::startRenderPass(renderPass, renderInfo);

	Render::clearView(1.0, 1.0, 1.0, 1.0);

	basePass(renderPass, renderInfo);

	Render::endRenderPass(renderPass);

	Render::unUseFrameBuffer();
}

FrameBuffer* getForwardBasePassFrameBuffer()
{
	return ForwardBasePass::forwardBasePassFrameBuffer;
}
