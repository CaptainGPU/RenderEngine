//
// Created by Eugene Karpenko @ CaptainGPU
// https://twitter.com/CaptainGPU
//
#include "renderPass.hxx"
#include "render.hxx"

RenderPass::RenderPass():
m_passProgramm(nullptr)
{

}

void RenderPass::makeProgram()
{
	VertexShader* vShader = Render::createVertexShader();
	FragmentShader* fShader = Render::createFragmentShader();

	m_passProgramm = Render::createPassProgramm(vShader, fShader);
}

void RenderPass::startRenderPass()
{
}

void RenderPass::endRenderPass()
{
}

PassProgramm* RenderPass::getPassProgramm()
{
	return m_passProgramm;
}

