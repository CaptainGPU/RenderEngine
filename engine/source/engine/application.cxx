#include "engine/application.hxx"
#include <istream>

#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"

void startApplication()
{
    printf("start engine!\n");
    
    GLFWwindow* window = nullptr;
    
    if (!glfwInit()) 
    {
        printf("GLFW: Init error!\n");
    }
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    
    window = glfwCreateWindow(800, 600, "Render Engine GLFW", nullptr, nullptr);
    
    if (!window) 
    {
        printf("GLFW: Create window problem\n");
        glfwTerminate();
    }
    
    glfwMakeContextCurrent(window);
    
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
    }
    
    glfwTerminate();
}
