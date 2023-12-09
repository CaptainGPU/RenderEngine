//
// Created by Eugene Karpenko @ CaptainGPU
// https://twitter.com/CaptainGPU
//
#pragma once

class PassProgramm;

class RenderPass
{
public:
	RenderPass();

	void makeProgram();

	void startRenderPass();
	void endRenderPass();

	PassProgramm* getPassProgramm();

private:
	PassProgramm* m_passProgramm;
};
