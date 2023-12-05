#include "render.hxx"

#include <iostream>

static const char* vShader = "                                         \n\
#version 330                                                           \n\
layout (location = 0) in vec3 pos;                                      \n\
                                                                       \n\
out vec3 vertexColor;                                                      \n\
void main()                                                            \n\
{                                                                      \n\
    vertexColor = vec3(1.0, .0, .0);                               \n\
    vec3 weights[3] = vec3[3](vec3(1.0, 0.0, 0.0), vec3(0.0, 1.0, 0.0), vec3(0.0, 0.0, 1.0)); \n\
    vertexColor = weights[gl_VertexID];         \n\
    gl_Position = vec4(0.9f * pos.x, 0.9f * pos.y, pos.z, 1.0f);       \n\
}";

static const char* fShader = "                      \n\
#version 330                                        \n\
in vec3 vertexColor;                                \n\
out vec4 color;                                     \n\
                                                    \n\
void main()                                         \n\
{                                                   \n\
    color =  vec4(vertexColor, 1.0f);     \n\
}                                                   \n\
";

GLuint Render::VAO;
GLuint Render::VBO;
GLuint Render::shader;

void Render::init()
{
	createTriangle();
	compileShaders();
}

void Render::setViewport(int x, int y, int width, int height)
{
	glViewport(x, y, width, height);
}

void Render::draw()
{
	glClearColor(.0, .5, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(shader);

	glBindVertexArray(VAO);

	glDrawArrays(GL_TRIANGLES, 0, 3);

	glBindVertexArray(0);
	glUseProgram(0);
}

void Render::createTriangle()
{
	GLfloat vertices[] = {
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		0.0f, 1.0f, 0.0f
	};

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Render::addShader(GLuint program, const char* shaderCode, GLenum shaderType)
{
	GLuint theShader = glCreateShader(shaderType);

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
		printf("Error compiling the %d shader: %s\n", shaderType, errorLog);
		return;
	}

	glAttachShader(program, theShader);
}

void Render::compileShaders()
{
	// make program
	shader = glCreateProgram();

	if (!shader)
	{
		printf("Error: Creating shader program!\n");
		return;
	}

	// add shader code to programm
	addShader(shader, vShader, GL_VERTEX_SHADER);
	addShader(shader, fShader, GL_FRAGMENT_SHADER);

	GLint result = 0;
	GLchar errorLog[128] = { 0 };

	// compiling programm
	glLinkProgram(shader);
	glGetProgramiv(shader, GL_LINK_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shader, sizeof(errorLog), nullptr, errorLog);
		printf("Error linking program: %s\n", errorLog);
		return;
	}

	// validation programm
	glValidateProgram(shader);
	glGetProgramiv(shader, GL_VALIDATE_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shader, sizeof(errorLog), nullptr, errorLog);
		printf("Error validation program: %s\n", errorLog);
		return;
	}
}