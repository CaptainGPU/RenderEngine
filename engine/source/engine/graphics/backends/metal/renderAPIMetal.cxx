#include "engine/graphics/backends/metal/renderAPIMetal.hxx"

#include "engine/adapters/glfw_adapter.h"
#include "engine/window/backends/glfw/glfwWindow.hxx"

RenderAPIMetal::RenderAPIMetal(Window* window) :
RenderAPI(window)
{
    mDevice = MTL::CreateSystemDefaultDevice();
    
    mMetalLayer = CA::MetalLayer::layer()->retain();
    mMetalLayer->setDevice(mDevice);
    mMetalLayer->setPixelFormat(MTL::PixelFormat::PixelFormatBGRA8Unorm);
    
    mCommandQueue = mDevice->newCommandQueue();
    
    setWindow(window);
}


void RenderAPIMetal::setWindow(Window* window)
{
    GLFWWindow* glfwWindow = dynamic_cast<GLFWWindow*>(window);
    mNsWindow = get_ns_window(glfwWindow->getGLFLWindow(), mMetalLayer);;
}

void RenderAPIMetal::renderFrame()
{
    NS::AutoreleasePool* pool = NS::AutoreleasePool::alloc()->init();
    mMetalDrawable = mMetalLayer->nextDrawable();
    MTL::CommandBuffer* commandBuffer = mCommandQueue->commandBuffer();
    MTL::RenderPassDescriptor* renderPassDescriptor = MTL::RenderPassDescriptor::alloc()->init();
    MTL::RenderPassColorAttachmentDescriptor* colorAttachments = renderPassDescriptor->colorAttachments()->object(0);
    colorAttachments->setTexture(mMetalDrawable->texture());
    colorAttachments->setLoadAction(MTL::LoadAction::LoadActionClear);
    colorAttachments->setClearColor(MTL::ClearColor(0.0f, 0.5f, 1.0f, 1.0f));
    colorAttachments->setStoreAction(MTL::StoreAction::StoreActionStore);
    
    MTL::RenderCommandEncoder* renderCommandEncouder = commandBuffer->renderCommandEncoder(renderPassDescriptor);
    renderCommandEncouder->endEncoding();
    
    commandBuffer->presentDrawable(mMetalDrawable);
    commandBuffer->commit();
    commandBuffer->waitUntilCompleted();
    
    renderPassDescriptor->release();
    pool->release();
    
//    NS::AutoreleasePool* pool = NS::AutoreleasePool::alloc()->init();
//    
//    mMetalDrawable = mMetalLayer->nextDrawable();
//    MTL::CommandBuffer* commandBuffer = mCommandQueue->commandBuffer();
//    MTL::RenderPassDescriptor* renderPass = MTL::RenderPassDescriptor::alloc()->init();
//    MTL::RenderPassColorAttachmentDescriptor* colorAttachments = renderPass->colorAttachments()->object(0);
//    colorAttachments->setTexture(mMetalDrawable->texture());
//    colorAttachments->setLoadAction(MTL::LoadAction::LoadActionClear);
//    colorAttachments->setClearColor(MTL::ClearColor(0.0f, 0.5f, 1.0f, 1.0f));
//    colorAttachments->setStoreAction(MTL::StoreAction::StoreActionStore);
//
//    MTL::RenderCommandEncoder* renderCommandEncouder = commandBuffer->renderCommandEncoder(renderPass);
//    renderCommandEncouder->endEncoding();
//
//    commandBuffer->presentDrawable(mMetalDrawable);
//    commandBuffer->commit();
//    commandBuffer->waitUntilCompleted();
//    
//    renderPass->release();
//    pool->release();
}

RenderAPIMetal::~RenderAPIMetal()
{
    
}
