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
layout (location = 1) in vec2 tex; \n\
layout (location = 2) in vec3 norm; \n\
                                                                       \n\
out vec3 vertexColor;                                                      \n\
uniform mat4 modelMatrix; \n\
uniform mat4 viewMatrix; \n\
uniform mat4 projectionMatrix; \n\
void main()                                                            \n\
{                                                                      \n\
    vertexColor = vec3(1.0, .0, .0);                               \n\
    vec3 weights[3] = vec3[3](vec3(1.0, 0.0, 0.0), vec3(0.0, 1.0, 0.0), vec3(0.0, 0.0, 1.0)); \n\
    vertexColor = norm;         \n\
    gl_Position = projectionMatrix * modelMatrix * vec4(pos, 1.0f);       \n\
}";

#else

static const char* vShader = "#version 330                                                          \n\
precision highp float;\n\
layout (location = 0) in vec3 pos;                                      \n\
layout (location = 1) in vec2 tex; \n\
layout (location = 2) in vec3 norm; \n\
                                                                       \n\
out vec3 vertexColor;                                                      \n\
uniform mat4 modelMatrix; \n\
uniform mat4 viewMatrix; \n\
uniform mat4 projectionMatrix; \n\
void main()                                                            \n\
{                                                                      \n\
    vertexColor = vec3(1.0, .0, .0);                               \n\
    vec3 weights[3] = vec3[3](vec3(1.0, 0.0, 0.0), vec3(0.0, 1.0, 0.0), vec3(0.0, 0.0, 1.0)); \n\
    vertexColor = vec3(tex, 1.0 - tex.y);         \n\
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(pos, 1.0f);       \n\
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

// Viewport size setting function
void Render::setViewport(int x, int y, int width, int height)
{
	// Call OpenGL glViewport
	glViewport(x, y, width, height);
}

void Render::clearView(float r, float g, float b, float a)
{
	glClearColor(r, g, b, a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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

void Render::deletePassProgramm(PassProgramm* programm)
{
    GLuint oglProgramm = programm->get_OpenGL_Programm();
    glDeleteProgram(oglProgramm);
    
    programm->finish();
    delete programm;
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

void Render::startRenderPass(RenderPass* renderPass,RenderInfo& info)
{
    addRenderPass(info);
    
    glEnable(GL_DEPTH_TEST);
	clearView(1.0, 1.0, 1.0, 1.0);

	PassProgramm* programm = renderPass->getPassProgramm();

	usePassProgramm(programm);

}

void Render::endRenderPass(RenderPass* renderPass)
{
	unBindVAO();
	unUsePassProgramm();
}

void Render::drawMesh(Mesh* mesh, RenderInfo& info)
{
	//glBindVertexArray(VAO);
    bindVAO(mesh->getVAO());
    bindEBO(mesh->getEBO());
    uint32_t numIndices = mesh->getEBO()->getNumIndices();
	glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, 0);
    unBindEBO();
    unBindVAO();
    
    addVertex(info, mesh->getNumVertex());
    addIndices(info, mesh->getEBO()->getNumIndices());
    addRenderObject(info);
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

void Render::createVBO(Mesh* mesh)
{
    float vertices[] = {
        -1.0f, 1.0f, -1.0f,        0.0f, 1.0f,        1.0f, 0.0f, 0.0f,
        -1.0f, -1.0f, -1.0f,        0.0f, 0.0f,        0.0f, 1.0f, 0.0f,
        1.0f, 1.0f, -1.0f,        1.0f, 1.0f,        0.0f, 0.0f, 1.0f,
        1.0f, -1.0f, -1.0f,        1.0f, 0.0f,        1.0f, 1.0f, 0.0f,

        -1.0f, 1.0f, 1.0f,        0.0f, 1.0f,        0.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 1.0f,        1.0f, 1.0f,        0.0f, 0.0f, 1.0f,
        -1.0f, -1.0f, 1.0f,        0.0f, 0.0f,        1.0f, 0.0f, 0.0f,
        1.0f, -1.0f, 1.0f,        1.0f, 0.0f,        1.0f, 0.0f, 1.0f
    };
    unsigned int indices[] = {  // note that we start from 0!
        // front
        0, 1, 2,
        2, 1, 3,
        // right
        2, 3, 5,
        5, 3, 7,
        // back
        5, 7, 4,
        4, 7, 6,
        // left
        4, 6, 0,
        0, 6, 1,
        // top
        4, 0, 5,
        5, 0, 2,
        // bottom
        1, 6, 3,
        3, 6, 7
    };

    VertexAttributeObject* vao = new VertexAttributeObject();
    vao->init();
    
    ElementBufferObject* ebo = new ElementBufferObject(sizeof(indices) / sizeof(unsigned int));
    ebo->init();

    GLuint vbo1, vao1, ebo1;
    vao1 = vao->getOpenGLVAO();
    ebo1 = ebo->get_OpenGL_EBO();

    Render::bindVAO(vao);

    glGenBuffers(1, &vbo1);
    
    glBindBuffer(GL_ARRAY_BUFFER, vbo1);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo1);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // XYZ data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 8, 0);
    glEnableVertexAttribArray(0);

    // UV data
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 8, (void*)(sizeof(vertices[0])*3));
    glEnableVertexAttribArray(1);

    // normals
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 8, (void*)(sizeof(vertices[0]) * 5));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    mesh->set_OpenGL_VBO(vbo1);
    mesh->setVAO(vao);
    mesh->setEBO(ebo);
    mesh->setNumVertex(sizeof(vertices) / (sizeof(float) * 8));
}

void Render::deleteVBO(Mesh* mesh)
{
    ElementBufferObject* ebo = mesh->getEBO();
    VertexAttributeObject* vao = mesh->getVAO();
    
	GLuint vbo = mesh->get_OpenGL_VBO();
	glDeleteBuffers(1, &vbo);
    
    Render::deleteEBO(ebo);
    Render::deleteVAO(vao);
}

void Render::createEBO(ElementBufferObject* ebo)
{
    GLuint oglEbo;
    glGenBuffers(1, &oglEbo);
    
    ebo->set_OpenGL_EBO(oglEbo);
}

void Render::deleteEBO(ElementBufferObject* ebo)
{
    GLuint oglEbo = ebo->get_OpenGL_EBO();
    glDeleteBuffers(1, &oglEbo);
}

void Render::bindEBO(ElementBufferObject* ebo)
{
    GLuint oglEbo = ebo->get_OpenGL_EBO();
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, oglEbo);
}

void Render::unBindEBO()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

Uniform* Render::getUniformFromPassProgramm(std::string uniformName, PassProgramm* programm)
{
    GLuint oglProgramm = programm->get_OpenGL_Programm();
    usePassProgramm(programm);
    
    GLint location = glGetUniformLocation(oglProgramm, uniformName.c_str());
    
    unUsePassProgramm();
    
    if (location < 0)
    {
        return nullptr;
    }
    
    Uniform* uniform = new Uniform();
    uniform->set_OpenGL_uniformID(location);
    
    
    return uniform;
}

void Render::setUniformMatrix4x4(Uniform* uniform, glm::mat4& matrix)
{
    GLint id = uniform->get_OpenGL_uniformID();
    
    glUniformMatrix4fv(id, 1, GL_FALSE, glm::value_ptr(matrix));
}
