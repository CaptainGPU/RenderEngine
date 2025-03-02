#include <cassert>

#include "engine/defines.hxx"

#include "engine/graphics/render.hxx"

#if GRAPHICS_API == GRAPHICS_API_METAL
#include "engine/graphics/backends/metal/renderAPIMetal.hxx"
#else
#include "engine/graphics/renderAPI.hxx"
#endif

Render* Render::mRender = nullptr;

void Render::initRenderAPI()
{
    if (!mRender)
    {
        mRender = new Render();
    }
    else
    {
        assert("Render: Init Render API problem/n");
    }
}

Render* Render::get()
{
    return Render::mRender;
}

Render::Render()
{
#if GRAPHICS_API == GRAPHICS_API_METAL
    mRenderAPI = new RenderAPIMetal();
#else
    mRenderAPI = new RenderAPI();
#endif
}

void Render::setWindow(Window* window)
{
    mRenderAPI->setWindow(window);
}

void Render::renderFrame()
{
    mRenderAPI->renderFrame();
}
