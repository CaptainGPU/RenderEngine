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
#include "mesh.hxx"

#include "vertexShader.hxx"
#include "fragmentShader.hxx"

#include "passProgramm.hxx"

#include "renderPass.hxx"

// Static class that contains basic rendering functions
class Render
{
public:
	// Render initialization function
	static void init();

	// Viewport size setting function
	static void setViewport(int x, int y, int width, int height);

	// Temporary drawing function, TODO: Will be deleted
	static void draw();


	// Render functions

	static void createVAO(VertexAttributeObject* attributeObject);
	static void deleteVAO(VertexAttributeObject* attributeObject);
	static void bindVAO(VertexAttributeObject* attributeObject);
	static void unBindVAO();

	static void createVBO(Mesh* mesh);
	static void deleteVBO(Mesh* mesh);

	static void clearView(float r, float g, float b, float a);

	static VertexShader* createVertexShader();
	static FragmentShader* createFragmentShader();

	static PassProgramm* createPassProgramm(VertexShader* vShader, FragmentShader* fShader);
    static void deletePassProgramm(PassProgramm* programm);
	static void usePassProgramm(PassProgramm* programm);
	static void unUsePassProgramm();

	static void startRenderPass(RenderPass* renderPass);
	static void endRenderPass(RenderPass* renderPass);

	static void drawMesh(Mesh* mesh);
};
