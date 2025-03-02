#pragma once

#include "engine/window/window.hxx"

class GLFWwindow;

class GLFWWindow: public Window
{
public:
    GLFWWindow(uint32_t width, uint32_t height);
    ~GLFWWindow();
    
    void frame() override;
    bool isShouldClose() override;
    void close() override;
    
    GLFWwindow* getGLFLWindow();
    
private:
    GLFWwindow* mWindow = nullptr;
};
