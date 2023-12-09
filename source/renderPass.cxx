//
// Created by Eugene Karpenko @ CaptainGPU
// https://twitter.com/CaptainGPU
//
#include "renderPass.hxx"
#include "render.hxx"

RenderPass::RenderPass():
m_passProgramm(nullptr),
m_vao(nullptr)
{

}

void RenderPass::makeProgram(VertexAttributeObject* vao)
{
	VertexShader* vShader = Render::createVertexShader();
	FragmentShader* fShader = Render::createFragmentShader();

	m_passProgramm = Render::createPassProgramm(vShader, fShader);

	m_vao = vao;
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

VertexAttributeObject* RenderPass::getVAO()
{
	return m_vao;
}
