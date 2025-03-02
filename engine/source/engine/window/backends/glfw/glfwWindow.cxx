#include <iostream>

#include "engine/window/backends/glfw/glfwWindow.hxx"

#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"


GLFWWindow::GLFWWindow(uint32_t width, uint32_t height) :
Window(width, height),
mWindow(nullptr)
{
    if (!glfwInit())
    {
        printf("GLFW: Init error!\n");
    }
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    
    mWindow = glfwCreateWindow(width, height, "Render Engine GLFW", nullptr, nullptr);
    
    if (!mWindow)
    {
        printf("GLFW: Create window problem\n");
        glfwTerminate();
    }
    
//    MTL::Device* device = MTL::CreateSystemDefaultDevice();
//    
//    CA::MetalLayer* metalLayer = CA::MetalLayer::layer()->retain();
//    metalLayer->setDevice(device);
//    metalLayer->setPixelFormat(MTL::PixelFormat::PixelFormatBGRA8Unorm);
//    
//    NS::Window* nsWindow = get_ns_window(mWindow, metalLayer);
//    
//    CA::MetalDrawable* metalDrawable;
//    MTL::CommandQueue* commandQueue = device->newCommandQueue()->retain();
//    
//    while (!glfwWindowShouldClose(mWindow))
//    {
//        
//        
//        NS::AutoreleasePool* pool = NS::AutoreleasePool::alloc()->init();
//        
//        metalDrawable = metalLayer->nextDrawable();
//        MTL::CommandBuffer* commandBuffer = commandQueue->commandBuffer();
//        MTL::RenderPassDescriptor* renderPass = MTL::RenderPassDescriptor::alloc()->init();
//        MTL::RenderPassColorAttachmentDescriptor* colorAttachments = renderPass->colorAttachments()->object(0);
//        colorAttachments->setTexture(metalDrawable->texture());
//        colorAttachments->setLoadAction(MTL::LoadAction::LoadActionClear);
//        colorAttachments->setClearColor(MTL::ClearColor(0.75f, 0.25f, 0.25f, 1.0f));
//        colorAttachments->setStoreAction(MTL::StoreAction::StoreActionStore);
//        
//        MTL::RenderCommandEncoder* renderCommandEncouder = commandBuffer->renderCommandEncoder(renderPass);
//        renderCommandEncouder->endEncoding();
//        
//        commandBuffer->presentDrawable(metalDrawable);
//        commandBuffer->commit();
//        commandBuffer->waitUntilCompleted();
//        
//        pool->release();
//    }
//    
//    commandQueue->release();
//    nsWindow->release();
//    metalLayer->release();
    
    
}

void GLFWWindow::frame()
{
    glfwPollEvents();
}

void GLFWWindow::close()
{
    glfwTerminate();
}

GLFWWindow::~GLFWWindow()
{
    
}

bool GLFWWindow::isShouldClose()
{
    return glfwWindowShouldClose(mWindow);
}

GLFWwindow* GLFWWindow::getGLFLWindow()
{
    return mWindow;
}
