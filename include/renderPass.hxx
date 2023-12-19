//
// Created by Eugene Karpenko @ CaptainGPU
// https://twitter.com/CaptainGPU
//
#pragma once

#include <vector>
#include <string>
#include <map>

class PassProgramm;

class Uniform;

class RenderPass
{
public:
	RenderPass();

	void makeProgram(std::string vertexProgram, std::string fragmentProgram);
    
    void registerUniforms(std::vector<std::string>& uniformsNames);

	PassProgramm* getPassProgramm();
    
    Uniform* getUniform(std::string name);
    
    void finish();

    void setTwoSideRender(bool isTwoSide);
    bool isTwoSideRender();

    void setWireFrameRender(bool isWireFrame);
    bool isWireFrameRender();

private:
	PassProgramm* m_passProgramm;
    std::map<std::string, Uniform*> m_uniforms;
    bool m_twoSideRender;
    bool m_wireFrameRender;
};
