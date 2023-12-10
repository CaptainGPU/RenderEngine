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

	PassProgramm* getPassProgramm();
    
    void finish();

private:
	PassProgramm* m_passProgramm;
};
