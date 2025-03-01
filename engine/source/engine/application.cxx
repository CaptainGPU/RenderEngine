#include "engine/application.hxx"
#include <istream>

#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"

#define NS_PRIVATE_IMPLEMENTATION
#define MTL_PRIVATE_IMPLEMENTATION
#define CA_PRIVATE_IMPLEMENTATION

#include <Foundation/Foundation.hpp>
#include <Metal/Metal.hpp>
#include <QuartzCore/QuartzCore.hpp>
#include <AppKit/AppKit.hpp>

#include "engine/adapters/glfw_adapter.h"

void startApplication()
{
    printf("start engine!\n");
    
    GLFWwindow* window = nullptr;
    
    if (!glfwInit()) 
    {
        printf("GLFW: Init error!\n");
    }
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    
    window = glfwCreateWindow(800, 600, "Render Engine GLFW", nullptr, nullptr);
    
    if (!window)
    {
        printf("GLFW: Create window problem\n");
        glfwTerminate();
    }
    
    MTL::Device* device = MTL::CreateSystemDefaultDevice();
    
    CA::MetalLayer* metalLayer = CA::MetalLayer::layer()->retain();
    metalLayer->setDevice(device);
    metalLayer->setPixelFormat(MTL::PixelFormat::PixelFormatBGRA8Unorm);
    
    NS::Window* nsWindow = get_ns_window(window, metalLayer);
    
    CA::MetalDrawable* metalDrawable;
    MTL::CommandQueue* commandQueue = device->newCommandQueue()->retain();
    
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        
        NS::AutoreleasePool* pool = NS::AutoreleasePool::alloc()->init();
        
        metalDrawable = metalLayer->nextDrawable();
        MTL::CommandBuffer* commandBuffer = commandQueue->commandBuffer();
        MTL::RenderPassDescriptor* renderPass = MTL::RenderPassDescriptor::alloc()->init();
        MTL::RenderPassColorAttachmentDescriptor* colorAttachments = renderPass->colorAttachments()->object(0);
        colorAttachments->setTexture(metalDrawable->texture());
        colorAttachments->setLoadAction(MTL::LoadAction::LoadActionClear);
        colorAttachments->setClearColor(MTL::ClearColor(0.75f, 0.25f, 0.25f, 1.0f));
        colorAttachments->setStoreAction(MTL::StoreAction::StoreActionStore);
        
        MTL::RenderCommandEncoder* renderCommandEncouder = commandBuffer->renderCommandEncoder(renderPass);
        renderCommandEncouder->endEncoding();
        
        commandBuffer->presentDrawable(metalDrawable);
        commandBuffer->commit();
        commandBuffer->waitUntilCompleted();
        
        pool->release();
    }
    
    commandQueue->release();
    nsWindow->release();
    metalLayer->release();
    
    glfwTerminate();
}
