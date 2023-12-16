//
// Created by Eugene Karpenko @ CaptainGPU
// https://twitter.com/CaptainGPU
//

#include "shaderLoader.hxx"
#include "defines.hxx"

#include <fstream>

#if CURRENT_PLATFORM == PLATFORM_EMSCRIPTEN
#else
#include <glad/gl.h>
#endif

std::string glslCode(std::string fileLocation)
{
	std::string content;
	std::ifstream fileStream(fileLocation, std::ios::in);

	if (!fileStream.is_open())
	{
		printf("File to read: %s doesn't exist\n", fileLocation.c_str());
		return "";
	}

	std::string line = "";
	while (!fileStream.eof())
	{
		std::getline(fileStream, line);
		content.append(line + "\n");
	}

	fileStream.close();
	return content;
}

std::string getShaderPath(std::string name)
{
	std::string platform;
    
    std::string location;
    
#if CURRENT_PLATFORM == PLATFORM_MAC
    location = "../../";
#else
    location = "../";
#endif

#if CURRENT_PLATFORM == PLATFORM_EMSCRIPTEN
    platform = "WebGL";
#else
	platform = "OpenGL";
#endif

	std::string path = location + std::string("shaders/") + platform + "/" + name;
	return path;
}

void loadVertexShader(VertexShader* shader, std::string name)
{
	std::string shaderPath = getShaderPath(name);

	std::string stringCode = glslCode(shaderPath);

	GLuint theShader = glCreateShader(GL_VERTEX_SHADER);

	const char* shaderCode = stringCode.c_str();
	const GLchar* code[1];
	code[0] = shaderCode;

	GLint codeLength[1];
	codeLength[0] = strlen(shaderCode);

	glShaderSource(theShader, 1, code, codeLength);
	glCompileShader(theShader);

	GLint result = 0;
	GLchar errorLog[128] = { 0 };

	// compile programm
	glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(theShader, sizeof(errorLog), nullptr, errorLog);
		printf("Error compiling the %d shader: %s\n", GL_VERTEX_SHADER, errorLog);

	}

	shader->set_OpenGL_Shader(theShader);
}

void loadFragmentShader(FragmentShader* shader, std::string name)
{

	std::string shaderPath = getShaderPath(name);

	std::string stringCode = glslCode(shaderPath);

	GLuint theShader = glCreateShader(GL_FRAGMENT_SHADER);

	const char* shaderCode = stringCode.c_str();
	const GLchar* code[1];
	code[0] = shaderCode;

	GLint codeLength[1];
	codeLength[0] = strlen(shaderCode);

	glShaderSource(theShader, 1, code, codeLength);
	glCompileShader(theShader);

	GLint result = 0;
	GLchar errorLog[128] = { 0 };

	// compile programm
	glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(theShader, sizeof(errorLog), nullptr, errorLog);
		printf("Error compiling the %d shader: %s\n", GL_FRAGMENT_SHADER, errorLog);

	}

	shader->set_OpenGL_Shader(theShader);

}
