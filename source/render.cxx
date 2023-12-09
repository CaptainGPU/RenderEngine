//
// Created by Eugene Karpenko @ CaptainGPU
// https://twitter.com/CaptainGPU
//

#include "render.hxx"

#include <iostream>

// Temporary vertex shader code, TODO: Will be deleted
#if CURRENT_PLATFORM == PLATFORM_EMSCRIPTEN

static const char* vShader = "#version 300 es \n\
precision highp float;\n\
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

#else

static const char* vShader = "#version 330                                                          \n\
precision highp float;\n\
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
#endif

#if CURRENT_PLATFORM == PLATFORM_EMSCRIPTEN
// Temporary fragment shader, TODO: Will be deleted
static const char* fShader = "#version 300 es\n\
precision highp float;\n\
in vec3 vertexColor;                                \n\
out vec4 color;                                     \n\
                                                    \n\
void main()                                         \n\
{                                                   \n\
    color =  vec4(vertexColor, 1.0f);     \n\
}                                                   \n\
";

#else
// Temporary fragment shader, TODO: Will be deleted
static const char* fShader = "#version 330                                       \n\
precision highp float;\n\
in vec3 vertexColor;                                \n\
out vec4 color;                                     \n\
                                                    \n\
void main()                                         \n\
{                                                   \n\
    color =  vec4(vertexColor, 1.0f);     \n\
}                                                   \n\
";

#endif

// Temporary variables, since all that Render can do now is display a triangle, TODO: Will be deleted
/*GLuint Render::VAO;
GLuint Render::VBO;
GLuint Render::shader;*/

// Render initialization function
/*void Render::init()
{
	glGenVertexArrays(1, &VAO);
	// Temporary code for creating the triangle and shaders to display it, TODO: Will be deleted
	createTriangle();
	compileShaders();
}*/

// Viewport size setting function
void Render::setViewport(int x, int y, int width, int height)
{
	// Call OpenGL glViewport
	glViewport(x, y, width, height);
}

// Temporary drawing function, TODO: Will be deleted
// Displays triangle only
/*void Render::draw()
{
	glClearColor(.0, .5, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(shader);

	glBindVertexArray(VAO);

	glDrawArrays(GL_TRIANGLES, 0, 3);

	glBindVertexArray(0);
	glUseProgram(0);
}*/

void Render::clearView(float r, float g, float b, float a)
{
	glClearColor(r, g, b, a);
	glClear(GL_COLOR_BUFFER_BIT);
}

/**/

VertexShader* Render::createVertexShader()
{
	VertexShader* shader = new VertexShader();

	GLuint theShader = glCreateShader(GL_VERTEX_SHADER);

	const char* shaderCode = vShader;
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

	return shader;
}

FragmentShader* Render::createFragmentShader()
{
	FragmentShader* shader = new FragmentShader();

	GLuint theShader = glCreateShader(GL_FRAGMENT_SHADER);

	const char* shaderCode = fShader;
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

	return shader;
}

PassProgramm* Render::createPassProgramm(VertexShader* vShader, FragmentShader* fShader)
{
	PassProgramm* programm = new PassProgramm();

	GLuint oglProgram = glCreateProgram();
	GLuint oglShaderV = vShader->get_OpenGL_Shader();
	GLuint oglShaderF = fShader->get_OpenGL_Shader();


	if (!oglProgram)
	{
		printf("Error: Creating pass program!\n");

		return programm;
	}

	glAttachShader(oglProgram, oglShaderV);
	glAttachShader(oglProgram, oglShaderF);

	GLint result = 0;
	GLchar errorLog[128] = { 0 };

	// Compiling programm
	glLinkProgram(oglProgram);
	glGetProgramiv(oglProgram, GL_LINK_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(oglProgram, sizeof(errorLog), nullptr, errorLog);
		printf("Error linking program: %s\n", errorLog);
		return programm;
	}

	programm->set_OpenGL_Programm(oglProgram);

	return programm;
}

void Render::usePassProgramm(PassProgramm* programm)
{
	glUseProgram(programm->get_OpenGL_Programm());
	//glUseProgram(shader);
}

void Render::unUsePassProgramm()
{
	glUseProgram(0);
}

void Render::startRenderPass(RenderPass* renderPass)
{
	clearView(1.0, 1.0, 1.0, 1.0);

	PassProgramm* programm = renderPass->getPassProgramm();

	usePassProgramm(programm);

	bindVAO(renderPass->getVAO());

}

void Render::endRenderPass(RenderPass* renderPass)
{
	unBindVAO();
	unUsePassProgramm();
}

void Render::drawMesh(Mesh* mesh)
{
	//glBindVertexArray(VAO);
	//glBindVertexArray(mesh->getVAO()->getOpenGLVAO());
	glDrawArrays(GL_TRIANGLES, 0, 3);
}

void Render::createVAO(VertexAttributeObject* attributeObject)
{
	GLuint vao;
	glGenVertexArrays(1, &vao);

	attributeObject->setOpenGLVAO(vao);
}

void Render::deleteVAO(VertexAttributeObject* attributeObject)
{
	GLuint vao = attributeObject->getOpenGLVAO();
	glDeleteVertexArrays(1, &vao);
}

void Render::bindVAO(VertexAttributeObject* attributeObject)
{
	GLuint vao = attributeObject->getOpenGLVAO();
	glBindVertexArray(vao);
}

void Render::unBindVAO()
{
	glBindVertexArray(0);
}

void Render::createVBO(Mesh* mesh, VertexAttributeObject* attributeObject)
{
	GLfloat vertices[] = {
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		0.0f, 1.0f, -1.0f
	};


	VertexAttributeObject* vao = attributeObject;
	VertexAttributeObject* vaoMesh = mesh->getVAO();

	if (!vaoMesh)
	{
		GLuint vbo1, vao1;
		vao1 = vao->getOpenGLVAO();

		Render::bindVAO(vao);

		glGenBuffers(1, &vbo1);
		glBindBuffer(GL_ARRAY_BUFFER, vbo1);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);


		mesh->set_OpenGL_VBO(vbo1);
		mesh->setVAO(vao);
	}

	/*VertexAttributeObject* meshVAO = mesh->getVAO();

	if (!attributeObject)
	{
		attributeObject = new VertexAttributeObject();

		GLfloat vertices[] = {
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		0.0f, 1.0f, -1.0f
		};

		GLuint vbo, vao;

		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		attributeObject->setOpenGLVAO(vao);
		mesh->set_OpenGL_VBO(vbo);

		mesh->setVAO(attributeObject);

		return;
	}*/


	/*VertexAttributeObject* vao = mesh->getVAO();
	if (vao || vao == attributeObject)
	{
		return;
	}

	vao = attributeObject;

	//unBindVAO();

	GLfloat vertices[] = {
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		0.0f, 1.0f, -1.0f
	};

	bindVAO(vao);

	GLuint vbo;
	glGenBuffers(1, &vbo);

	glBindBuffer(1, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	unBindVAO();

	mesh->setVAO(vao);
	mesh->set_OpenGL_VBO(vbo);*/
}

void Render::deleteVBO(Mesh* mesh)
{
	GLuint vbo = mesh->get_OpenGL_VBO();
	glDeleteBuffers(1, &vbo);
}

// Temporary triangle creation function, TODO: Will be deleted
/*void Render::createTriangle()
{
	GLfloat vertices[] = {
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		0.0f, 1.0f, -1.0f
	};

	
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

// Temporary OpenGL shader creation function, TODO: Will be deleted
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

// Temporary OpenGL shaders compilations function, TODO: Will be deleted
void Render::compileShaders()
{
	// Make program
	shader = glCreateProgram();

	if (!shader)
	{
		printf("Error: Creating shader program!\n");
		return;
	}
    
    glBindVertexArray(VAO);

	// Фdd shader code to programm
	addShader(shader, vShader, GL_VERTEX_SHADER);
	addShader(shader, fShader, GL_FRAGMENT_SHADER);

	GLint result = 0;
	GLchar errorLog[128] = { 0 };

	// Compiling programm
	glLinkProgram(shader);
	glGetProgramiv(shader, GL_LINK_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shader, sizeof(errorLog), nullptr, errorLog);
		printf("Error linking program: %s\n", errorLog);
		return;
	}

	// Validation programm
	glValidateProgram(shader);
	glGetProgramiv(shader, GL_VALIDATE_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shader, sizeof(errorLog), nullptr, errorLog);
		printf("Error validation program: %s\n", errorLog);
		return;
	}
}*/
