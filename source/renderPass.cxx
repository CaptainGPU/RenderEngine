//
// Created by Eugene Karpenko @ CaptainGPU
// https://twitter.com/CaptainGPU
//
#include "renderPass.hxx"
#include "render.hxx"
#include <iostream>

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

void RenderPass::registerUniforms(std::vector<std::string>& uniformsNames)
{
    for (std::string& uniformName : uniformsNames)
    {
        Uniform* uniform = Render::getUniformFromPassProgramm(uniformName, m_passProgramm);
        m_uniforms.insert(std::pair<std::string, Uniform*>(uniformName, uniform));
    }
}

Uniform* RenderPass::getUniform(std::string name)
{
    return m_uniforms.at(name);
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
