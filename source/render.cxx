//
// Created by Eugene Karpenko @ CaptainGPU
// https://twitter.com/CaptainGPU
//

#include "render.hxx"

#include "shaderLoader.hxx"
#include "meshLoader.hxx"
#include "engine.hxx"

#include <iostream>
#include <glm/gtc/type_ptr.hpp>

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

void Render::clearDepth()
{
    glClear(GL_DEPTH_BUFFER_BIT);
}

/**/

VertexShader* Render::createVertexShader(std::string vertexProgram)
{
	VertexShader* shader = new VertexShader();

    loadVertexShader(shader, vertexProgram + ".vert");

	return shader;
}

FragmentShader* Render::createFragmentShader(std::string fragmentProgram)
{
	FragmentShader* shader = new FragmentShader();

    loadFragmentShader(shader, fragmentProgram + ".frag");

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

void Render::useFrameBufferAttachment(FrameBuffer* frameBuffer)
{
    unsigned int attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
    glDrawBuffers(frameBuffer->getAttachmentCount(), attachments);
}

void Render::useFrameBuffer(FrameBuffer* frameBuffer)
{
    unsigned int fbo = frameBuffer->get_openGL_FBO();
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
}

void Render::unUseFrameBuffer()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Render::startRenderPass(RenderPass* renderPass,RenderInfo& info)
{
    addRenderPass(info);

    if (renderPass->isTwoSideRender())
    {
        glDisable(GL_CULL_FACE);
    }
    else
    {
        glEnable(GL_CULL_FACE);
    }

    glEnable(GL_DEPTH_TEST);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

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

void Render::drawMeshBound(MeshBound* bound, RenderInfo& info)
{
    bindVAO(bound->getVAO());
    bindEBO(bound->getEBO());
    uint32_t numIndices = bound->getEBO()->getNumIndices();
    glDrawElements(GL_LINES, numIndices, GL_UNSIGNED_INT, 0);
    unBindEBO();
    unBindVAO();
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

void Render::deleteVBO(Mesh* mesh)
{
    ElementBufferObject* ebo = mesh->getEBO();
    VertexAttributeObject* vao = mesh->getVAO();
    
	GLuint vbo = mesh->get_OpenGL_VBO();
	glDeleteBuffers(1, &vbo);
    
    Render::deleteEBO(ebo);
    //Render::deleteVAO(vao);
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
    
    Uniform* uniform = new Uniform();
    uniform->set_OpenGL_uniformID(location);
    
    
    return uniform;
}

void Render::setUniformVec3(Uniform* uniform, glm::vec3& value)
{
    GLint id = uniform->get_OpenGL_uniformID();

    glUniform3f(id, value.x, value.y, value.z);
}

void Render::setUniformVec4(Uniform* uniform, glm::vec4& value)
{
    GLint id = uniform->get_OpenGL_uniformID();

    glUniform4f(id, value.x, value.y, value.z, value.w);
}

void Render::setUniformMatrix4x4(Uniform* uniform, glm::mat4& matrix)
{
    GLint id = uniform->get_OpenGL_uniformID();
    
    glUniformMatrix4fv(id, 1, GL_FALSE, glm::value_ptr(matrix));
}

void Render::setUniformFloatValue(Uniform* uniform, float& value)
{
    GLint id = uniform->get_OpenGL_uniformID();
    if (id < 0)
    {
        return;
    }
    glUniform1f(id, value);
}

void Render::setUniformFloatArrayValue(Uniform* uniform, float* array, uint32_t size)
{
    GLint id = uniform->get_OpenGL_uniformID();
    if (array == nullptr)
    {
        return;
    }
    glUniform1fv(id, size, array);
}

void Render::setUniformIntValue(Uniform* uniform, int& value)
{
    GLint id = uniform->get_OpenGL_uniformID();
    if (id < 0)
    {
        return;
    }
    glUniform1i(id, value);
}

void Render::setUniformTexture(Uniform* uniform, Texture* texture, unsigned int slot)
{
    Render::activateTexture(texture, slot);
    Render::useTexture(texture);
    GLint id = uniform->get_OpenGL_uniformID();
    glUniform1i(id, slot);
}

void Render::setUniformCubeTexture(Uniform* uniform, Texture* texture, unsigned int slot)
{
    Render::activateTexture(texture, slot);
    Render::bindCubeMapTexture(texture);
    GLint id = uniform->get_OpenGL_uniformID();
    glUniform1i(id, slot);
}

FrameBuffer* Render::createFrameBuffer()
{
    Texture* texture = Render::createTexture();
    
    unsigned int width, height;

    Engine::get()->getWindowBufferSize(width, height);
    
    FrameBuffer* frameBuffer = new FrameBuffer();
    
    unsigned int fbo;
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    
    unsigned int format = GL_RGB16F;
    unsigned int chanels = GL_RGB;
    unsigned int dataFormat = GL_UNSIGNED_BYTE;
    
#if CURRENT_PLATFORM == PLATFORM_EMSCRIPTEN
    format = GL_SRGB8_ALPHA8;
    chanels = GL_RGBA;
    dataFormat = GL_UNSIGNED_BYTE;
#endif
    
    unsigned int frameBufferTexture = texture->get_OpenGL_Texture();
    Render::useTexture(texture);
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, chanels, dataFormat, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, frameBufferTexture, 0);
    
    unsigned int rbo;
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
    
    int fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    
    if (fboStatus != GL_FRAMEBUFFER_COMPLETE)
    {
        std::cout << "Framebuffer error:" << fboStatus << std::endl;
    }
    
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    Render::unUseTexture();
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
    
    frameBuffer->set_openGL_FBO(fbo);
    frameBuffer->setColorTexture(texture);
    
    return frameBuffer;
}

FrameBuffer* Render::createCustomFrameBuffer(unsigned int width, unsigned int height, unsigned int colorAttachents)
{
    FrameBuffer* frameBuffer = new FrameBuffer();

    unsigned int fbo;
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    unsigned int format = GL_RGBA16F;
    unsigned int chanels = GL_RGBA;
    unsigned int dataFormat = GL_FLOAT;

    for (size_t i = 0; i < colorAttachents; i++)
    {
        Texture* texture = Render::createTexture();
        unsigned int frameBufferTexture = texture->get_OpenGL_Texture();
        Render::useTexture(texture);

        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, chanels, dataFormat, nullptr);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, frameBufferTexture, 0);
        Render::unUseTexture();
        

        switch (i)
        {
        case 0: 
        {
            frameBuffer->setColorTexture(texture); 
            break;
        }
        case 1:
        {
            frameBuffer->setColorTexture1(texture);
            break;
        }

        case 2:
        {
            frameBuffer->setColorTexture2(texture);
            break;
        }

        default:
            break;
        }
    }

    Texture* depthMapTexture = Render::createTexture();

    unsigned int depthMap = depthMapTexture->get_OpenGL_Texture();
    Render::useTexture(depthMapTexture);

    unsigned int depthFormat = GL_DEPTH_COMPONENT;
    unsigned int depthTextureFormat = GL_FLOAT;
    unsigned int wrapMode = GL_CLAMP_TO_EDGE;

#if CURRENT_PLATFORM == PLATFORM_EMSCRIPTEN
    wrapMode = GL_CLAMP_TO_EDGE;
    depthFormat = GL_DEPTH_COMPONENT16;
    depthTextureFormat = GL_UNSIGNED_SHORT;
#endif

    glTexImage2D(GL_TEXTURE_2D, 0, depthFormat, width, height, 0, GL_DEPTH_COMPONENT, depthTextureFormat, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);

    Render::unUseTexture();

    int fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);

    if (fboStatus != GL_FRAMEBUFFER_COMPLETE)
    {
        std::cout << "Framebuffer error:" << fboStatus << std::endl;
    }

    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    Render::unUseTexture();
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    frameBuffer->set_openGL_FBO(fbo);
    frameBuffer->setDepthTexture(depthMapTexture);
    frameBuffer->setAttachmentCount(colorAttachents);

    return frameBuffer;
}

FrameBuffer* Render::createDepthMapFrameBuffer(unsigned int width, unsigned int height)
{
    FrameBuffer* frameBuffer = new FrameBuffer();

    unsigned int depthMapFBO;
    glGenFramebuffers(1, &depthMapFBO);

    //const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;

    Texture* depthMapTexture = Render::createTexture();

    unsigned int depthMap = depthMapTexture->get_OpenGL_Texture();
    Render::useTexture(depthMapTexture);

    unsigned int wrapMode;
    unsigned int depthFormat = GL_DEPTH_COMPONENT;
    unsigned int depthTextureFormat = GL_FLOAT;

#if CURRENT_PLATFORM == PLATFORM_EMSCRIPTEN
    wrapMode = GL_CLAMP_TO_EDGE;
    depthFormat = GL_DEPTH_COMPONENT16;
    depthTextureFormat = GL_UNSIGNED_SHORT;
#else
    wrapMode = GL_CLAMP_TO_BORDER;
#endif
    glTexImage2D(GL_TEXTURE_2D, 0, depthFormat, width, height, 0, GL_DEPTH_COMPONENT, depthTextureFormat, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode);

#if CURRENT_PLATFORM != PLATFORM_EMSCRIPTEN
    float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
#endif

    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);


//#if CURRENT_PLATFORM == PLATFORM_EMSCRIPTEN
    Texture* colorTexture = Render::createTexture();

    unsigned int format = GL_SRGB8_ALPHA8;
    unsigned int chanels = GL_RGBA;
    unsigned int dataFormat = GL_UNSIGNED_BYTE;

    unsigned int frameBufferTexture = colorTexture->get_OpenGL_Texture();
    Render::useTexture(colorTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, chanels, dataFormat, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, frameBufferTexture, 0);

//#endif
    /// <summary>
    /// 
    /// </summary>
    /// <returns></returns>

    
    //glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    Render::unUseTexture();

    frameBuffer->set_openGL_FBO(depthMapFBO);
    frameBuffer->setDepthTexture(depthMapTexture);
    frameBuffer->setColorTexture(colorTexture);

    return frameBuffer;
}

FrameBuffer* Render::createDepthCubeMapFrameBuffer(unsigned int width, unsigned int height)
{
    FrameBuffer* frameBuffer = new FrameBuffer();

    unsigned int FBO;
    glGenFramebuffers(1, &FBO);

    Texture* shadowCubeMap = createTexture();

    Render::bindCubeMapTexture(shadowCubeMap);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    unsigned int format = GL_RGBA;
    unsigned int chanels = GL_RGBA;
    unsigned int dataFormat = GL_UNSIGNED_BYTE;

    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, format, width, height, 0, chanels, dataFormat, nullptr);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, format, width, height, 0, chanels, dataFormat, nullptr);

    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, format, width, height, 0, chanels, dataFormat, nullptr);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, format, width, height, 0, chanels, dataFormat, nullptr);

    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, format, width, height, 0, chanels, dataFormat, nullptr);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, format, width, height, 0, chanels, dataFormat, nullptr);

    Render::unBindCubeMapTexture();

    Texture* shadowCubeMapDepth = createTexture();

    Render::bindCubeMapTexture(shadowCubeMapDepth);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    format = GL_DEPTH_COMPONENT;
    chanels = GL_DEPTH_COMPONENT;
    dataFormat = GL_FLOAT;

#if CURRENT_PLATFORM == PLATFORM_EMSCRIPTEN
    format = GL_DEPTH_COMPONENT16;
    dataFormat = GL_UNSIGNED_SHORT;
#endif

    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, format, width, height, 0, chanels, dataFormat, nullptr);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, format, width, height, 0, chanels, dataFormat, nullptr);

    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, format, width, height, 0, chanels, dataFormat, nullptr);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, format, width, height, 0, chanels, dataFormat, nullptr);

    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, format, width, height, 0, chanels, dataFormat, nullptr);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, format, width, height, 0, chanels, dataFormat, nullptr);

    Render::unBindCubeMapTexture();

    Render::useFrameBuffer(frameBuffer);

    /*unsigned int rbo;
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);*/

    int fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);

    if (fboStatus != GL_FRAMEBUFFER_COMPLETE)
    {
        std::cout << "Framebuffer error:" << fboStatus << std::endl;
    }

    Render::unUseFrameBuffer();

    frameBuffer->set_openGL_FBO(FBO);
    frameBuffer->setCubeTexture(shadowCubeMap);
    frameBuffer->setCubeDepthTexture(shadowCubeMapDepth);

    return frameBuffer;
}

Texture* Render::createTexture()
{
    printf("Render: create Texture \n");
    Texture* texture = new Texture();
    
    unsigned int oglTexture;
    glGenTextures(1, &oglTexture);
    
    texture->set_OpenGL_Texture(oglTexture);
    
    return texture;
}

void Render::useTexture(Texture* texture)
{
    unsigned int oglTexture = texture->get_OpenGL_Texture();
    glBindTexture(GL_TEXTURE_2D, oglTexture);
}

void Render::unUseTexture()
{
    glBindTexture(GL_TEXTURE_2D, 0);
    
}

void Render::activateTexture(Texture* texture, unsigned int slot)
{
    glActiveTexture(GL_TEXTURE0 + slot);
}

void Render::loadDataToTexture(Texture* texture, int width, int height, unsigned char* data)
{
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    //glGenerateMipmap(GL_TEXTURE_2D);
}

void Render::bindCubeMapTexture(Texture* texture)
{
    unsigned int oglTexture = texture->get_OpenGL_Texture();
    glBindTexture(GL_TEXTURE_CUBE_MAP, oglTexture);
}

void Render::unBindCubeMapTexture()
{
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

Texture* Render::makeNoiseLinearTexture(std::vector<glm::vec3>& data)
{
    Texture* texture = Render::createTexture();

    Render::useTexture(texture);

    unsigned int noiseTexture = texture->get_OpenGL_Texture();

    unsigned int format = GL_SRGB8_ALPHA8;
    unsigned int chanels = GL_RGBA;
    unsigned int dataFormat = GL_UNSIGNED_BYTE;

    glTexImage2D(GL_TEXTURE_2D, 0, format, 4, 4, 0, chanels, dataFormat, data.data());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    Render::unUseTexture();

    return texture;
}

Texture* Render::make3DTextureFloat(std::vector<float>& data, size_t width, size_t height, size_t depth)
{
    Texture* texture = createTexture();

    Render::bind3DTexture(texture);

    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    float* pixels = data.data();

    glTexImage3D(GL_TEXTURE_3D, 0, GL_R16F, width, height, height, 0, GL_RED, GL_FLOAT, pixels);

    Render::unBind3DTexture(texture);

    return texture;
}

void Render::bind3DTexture(Texture* texture)
{
    unsigned int oglTexture = texture->get_OpenGL_Texture();
    glBindTexture(GL_TEXTURE_3D, oglTexture);
}

void Render::unBind3DTexture(Texture* texture)
{
    glBindTexture(GL_TEXTURE_3D, 0);
}

void Render::setFrameBufferCubeSideRender(FrameBuffer* frameBuffer, unsigned int side)
{
    Texture* cubeTexture = frameBuffer->getCubeTexture();
    unsigned int texture = cubeTexture->get_OpenGL_Texture();
    cubeTexture = frameBuffer->getCubeDepthTexture();
    unsigned int depthTexture = cubeTexture->get_OpenGL_Texture();

    unsigned int sideOGL = GL_TEXTURE_CUBE_MAP_POSITIVE_X;

    switch (side)
    {
    case 0: sideOGL = GL_TEXTURE_CUBE_MAP_POSITIVE_X; break;
    case 1: sideOGL = GL_TEXTURE_CUBE_MAP_NEGATIVE_X; break;
    case 2: sideOGL = GL_TEXTURE_CUBE_MAP_POSITIVE_Z; break;
    case 3: sideOGL = GL_TEXTURE_CUBE_MAP_NEGATIVE_Z; break;
    case 4: sideOGL = GL_TEXTURE_CUBE_MAP_POSITIVE_Y; break;
    case 5: sideOGL = GL_TEXTURE_CUBE_MAP_NEGATIVE_Y; break;
    }

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, sideOGL, texture, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, sideOGL, depthTexture, 0);
}
