#include <cassert>

#include "engine/graphics/render.hxx"

#include "engine/graphics/backends/metal/renderAPIMetal.hxx"

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
    mRenderAPI = new RenderAPIMetal();
}

void Render::setWindow(Window* window)
{
    mRenderAPI->setWindow(window);
}

void Render::renderFrame()
{
    mRenderAPI->renderFrame();
}
