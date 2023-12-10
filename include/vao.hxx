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
#include <GLES3/gl3.h>
#endif

class VertexAttributeObject
{
public:
	VertexAttributeObject();
	virtual void init();
	virtual void finish();

	// OpeGL Staff
	void setOpenGLVAO(GLuint vao);
	GLuint getOpenGLVAO();
private:
	GLuint m_OGL_vao;
};