#pragma once

#include "glad/gl.h"

class Render
{
public:
	static void init();

	static void setViewport(int x, int y, int width, int height);

	static void draw();

private:
	static GLuint VBO;
	static GLuint VAO;
	static GLuint shader;

	static void createTriangle();
	static void addShader(GLuint program, const char* shaderCode, GLenum shaderType);
	static void compileShaders();
};