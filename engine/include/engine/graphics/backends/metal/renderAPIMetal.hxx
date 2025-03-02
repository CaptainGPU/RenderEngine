#pragma once

#include "engine/graphics/renderAPI.hxx"

//#define NS_PRIVATE_IMPLEMENTATION
//#define MTL_PRIVATE_IMPLEMENTATION
//#define CA_PRIVATE_IMPLEMENTATION

#include <Foundation/Foundation.hpp>
#include <Metal/Metal.hpp>
#include <QuartzCore/QuartzCore.hpp>
#include <AppKit/AppKit.hpp>

class Window;

class RenderAPIMetal : public RenderAPI
{
public:
    RenderAPIMetal();
    ~RenderAPIMetal();
    
    void setWindow(Window* window) override;
    void renderFrame() override;
private:
    MTL::Device* mDevice = nullptr;
    CA::MetalLayer* mMetalLayer = nullptr;
    NS::Window* mNsWindow = nullptr;
    CA::MetalDrawable* mMetalDrawable = nullptr;
    MTL::CommandQueue* mCommandQueue = nullptr;
};
