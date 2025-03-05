#pragma once

class RenderAPI;
class Window;

class Render
{
private:
    Render(Window* window);

private:
    RenderAPI* mRenderAPI = nullptr;
    
public:
    void renderFrame();

    void shootDownRenderAPI();
    
public:
    static void initRenderAPI(Window* window);
    static Render* get();
    
private:
    static Render* mRender;
};
