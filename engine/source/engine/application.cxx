#include "engine/application.hxx"
#include <istream>
#include "GLFW/glfw3.h"

void startApplication()
{
    printf("start engine!\n");
    
    GLFWwindow* window = nullptr;
    
    if (!glfwInit()) 
    {
        printf("GLFW: Init error!\n");
    }
    
    window = glfwCreateWindow(800, 600, "Render Engine GLFW", nullptr, nullptr);
    
    if (!window) 
    {
        printf("GLFW: Create window problem\n");
        glfwTerminate();
    }
    
    glfwMakeContextCurrent(window);
    
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    glfwTerminate();
}
