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

	static void clearView(float r, float g, float b, float a);

private:
	// Temporary variables, since all that Render can do now is display a triangle, TODO: Will be deleted
	static GLuint VBO;
	static GLuint VAO;
	static GLuint shader;

public:
	// Temporary triangle creation function, TODO: Will be deleted
	static void createTriangle();
	// Temporary OpenGL shader creation function, TODO: Will be deleted
	static void addShader(GLuint program, const char* shaderCode, GLenum shaderType);
	// Temporary OpenGL shaders compilations function, TODO: Will be deleted
	static void compileShaders();
};
