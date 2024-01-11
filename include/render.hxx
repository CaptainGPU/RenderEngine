//
// Created by Eugene Karpenko @ CaptainGPU
// https://twitter.com/CaptainGPU
//

#pragma once

#include "defines.hxx"

#if (CURRENT_PLATFORM == PLATFORM_WIN) || (CURRENT_PLATFORM == PLATFORM_MAC)
#include "glad/gl.h"
#endif

#if CURRENT_PLATFORM == PLATFORM_EMSCRIPTEN
#include <emscripten.h>
#include <GLES3/gl3.h>
#endif

#include "vao.hxx"
#include "ebo.hxx"
#include "mesh.hxx"

#include "vertexShader.hxx"
#include "fragmentShader.hxx"

#include "passProgramm.hxx"

#include "renderPass.hxx"
#include "renderInfo.hxx"

#include "uniform.hxx"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "frameBuffer.hxx"
#include "texture.hxx"

// Static class that contains basic rendering functions
class Render
{
public:
	// Render initialization function
	static void init();

	// Viewport size setting function
	static void setViewport(int x, int y, int width, int height);

	// Render functions
    
	static void createVAO(VertexAttributeObject* attributeObject);
	static void deleteVAO(VertexAttributeObject* attributeObject);
	static void bindVAO(VertexAttributeObject* attributeObject);
	static void unBindVAO();

	static void deleteVBO(Mesh* mesh);
    
    static void createEBO(ElementBufferObject* ebo);
    static void deleteEBO(ElementBufferObject* ebo);
    static void bindEBO(ElementBufferObject* ebo);
    static void unBindEBO();

	static void clearView(float r, float g, float b, float a);
	static void clearDepth();

	static VertexShader* createVertexShader(std::string vertexProgram);
	static FragmentShader* createFragmentShader(std::string fragmentProgram);

	static PassProgramm* createPassProgramm(VertexShader* vShader, FragmentShader* fShader);
    static void deletePassProgramm(PassProgramm* programm);
	static void usePassProgramm(PassProgramm* programm);
	static void unUsePassProgramm();
    
    static void useFrameBuffer(FrameBuffer* frameBuffer);
    static void unUseFrameBuffer();

	static void startRenderPass(RenderPass* renderPass, RenderInfo& info);
	static void endRenderPass(RenderPass* renderPass);
    
    static Uniform* getUniformFromPassProgramm(std::string uniformName, PassProgramm* programm);
	static void setUniformVec3(Uniform* uniform, glm::vec3& value);
    static void setUniformMatrix4x4(Uniform* uniform, glm::mat4& matrix);
	static void setUniformFloatValue(Uniform* uniform, float& value);
    static void setUniformIntValue(Uniform* uniform, int& value);
    static void setUniformTexture(Uniform* uniform, Texture* texture);
    
    static FrameBuffer* createFrameBuffer();

	static FrameBuffer* createDepthMapFrameBuffer();
    
    static Texture* createTexture();
    static void useTexture(Texture* texture);
    static void unUseTexture();
    static void activateTexture(Texture* texure, unsigned int slot);

	static void drawMesh(Mesh* mesh, RenderInfo& info);
	static void drawMeshBound(MeshBound* bound, RenderInfo& info);
};
