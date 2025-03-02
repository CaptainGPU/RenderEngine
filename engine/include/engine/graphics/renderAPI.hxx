#pragma once

class Window;

class RenderAPI
{
public:
    RenderAPI();
    
    virtual void setWindow(Window* window);
    virtual void renderFrame();
};
