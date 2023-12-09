//
// Created by Eugene Karpenko @ CaptainGPU
// https://twitter.com/CaptainGPU
//

#include <glad/gl.h>

class FragmentShader
{
public:
	FragmentShader();

	void set_OpenGL_Shader(GLuint shader);
	GLuint get_OpenGL_Shader();

private:
	GLuint m_shader;
};