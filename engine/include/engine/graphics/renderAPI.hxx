#pragma once

class Window;

class RenderAPI
{
public:
    RenderAPI(Window* window);
    virtual ~RenderAPI();
    virtual void renderFrame();
};
