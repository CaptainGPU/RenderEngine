//
// Created by Eugene Karpenko @ CaptainGPU
// https://twitter.com/CaptainGPU
//
#pragma once

#include "vao.hxx"

class PassProgramm;

class RenderPass
{
public:
	RenderPass();

	void makeProgram(VertexAttributeObject* vao);

	void startRenderPass();
	void endRenderPass();

	PassProgramm* getPassProgramm();

	VertexAttributeObject* getVAO();

private:
	PassProgramm* m_passProgramm;
	VertexAttributeObject* m_vao;
};