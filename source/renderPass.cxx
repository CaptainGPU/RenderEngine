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

PassProgramm* RenderPass::getPassProgramm()
{
	return m_passProgramm;
}

void RenderPass::finish()
{
    if (m_passProgramm)
    {
        Render::deletePassProgramm(m_passProgramm);
        m_passProgramm = nullptr;
    }
}
