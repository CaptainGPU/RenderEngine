// RenderEngine.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

//#include <OpenGL/gl3.h>

#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>


const GLint WIDTH = 800, HEIGHT = 600;

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

GLuint VBO, VAO, shader;

void CreateTriangle()
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


void AddShader(GLuint program, const char* shaderCode, GLenum shaderType)
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

void CompileShaders()
{
    // make program
    shader = glCreateProgram();

    if (!shader)
    {
        printf("Error: Creating shader program!\n");
        return;
    }

    // add shader code to programm
    AddShader(shader, vShader, GL_VERTEX_SHADER);
    AddShader(shader, fShader, GL_FRAGMENT_SHADER);

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

int main()
{

    if (!glfwInit())
    {
        printf("GLFW initialization failed");
        glfwTerminate();
        return 1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "Промінь", nullptr, nullptr);

    if (!mainWindow)
    {
        printf("GLFW window creation failed!");
        glfwTerminate();
        return 1;
    }

    int bufferWidth, bufferHeight;
    glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

    glfwMakeContextCurrent(mainWindow);

    int version = gladLoadGL(glfwGetProcAddress);

//    glewExperimental = GL_TRUE;
//
//    if (glewInit() != GLEW_OK)
//    {
//        printf("GLEW initialization failed!");
//        glfwDestroyWindow(mainWindow);
//        glfwTerminate();
//        return 1;
//    }
    
    IMGUI_CHECKVERSION();
      ImGui::CreateContext();
      ImGui_ImplGlfw_InitForOpenGL(mainWindow, true);
      ImGui_ImplOpenGL3_Init("#version 330 core");
      ImGui::StyleColorsClassic();

    glViewport(0, 0, bufferWidth, bufferHeight);

    CreateTriangle();
    CompileShaders();

    while (!glfwWindowShouldClose(mainWindow))
    {
        glfwPollEvents();

        glClearColor(.0, .5, 1.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shader);

        glBindVertexArray(VAO);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        glBindVertexArray(0);
        glUseProgram(0);
        
        
        ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            static bool showDemo = false;
            ImGui::Begin("Example");
            if (ImGui::Button("Show/Hide ImGui demo"))
              showDemo = !showDemo;
            ImGui::End();
            if (showDemo)
              ImGui::ShowDemoWindow(&showDemo);

            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(mainWindow);
    }
    
    ImGui_ImplOpenGL3_Shutdown();
     ImGui_ImplGlfw_Shutdown();
     ImGui::DestroyContext();
    
    glfwDestroyWindow(mainWindow);
      glfwTerminate();
      return 0;
}
