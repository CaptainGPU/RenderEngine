#pragma once

class RenderAPI;
class Window;

class Render
{
private:
    Render();

private:
    RenderAPI* mRenderAPI = nullptr;
    
public:
    void setWindow(Window* window);
    void renderFrame();
    
public:
    static void initRenderAPI();
    static Render* get();
    
private:
    static Render* mRender;
};
