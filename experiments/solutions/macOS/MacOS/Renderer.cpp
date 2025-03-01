//
//  Renderer.cpp
//  MacOS
//
//  Created by Eugene Karpenko on 26.11.2024.
//

#define NS_PRIVATE_IMPLEMENTATION
#define CA_PRIVATE_IMPLEMENTATION
#define MTL_PRIVATE_IMPLEMENTATION

#include <Foundation/Foundation.hpp>
#include <Metal/Metal.hpp>
#include <QuartzCore/QuartzCore.hpp>

#include "Renderer.hpp"
#include <iostream>

Renderer::Renderer(CA::MetalDrawable* const pDrawable, MTL::Device* const pDevice)
{
    _pDrawable = pDrawable;
    _pDevice = pDevice;
    _pCommandQueue = _pDevice->newCommandQueue();
    
    
}

Renderer::~Renderer()
{
    
}

void Renderer::draw() const
{
    MTL::CommandBuffer* pCmdBuffer = _pCommandQueue->commandBuffer();
    
    MTL::RenderPassDescriptor* pRpd = MTL::RenderPassDescriptor::alloc()->init();
    
    pRpd->colorAttachments()->object(0)->setTexture(_pDrawable->texture());
    pRpd->colorAttachments()->object(0)->setLoadAction(MTL::LoadAction::LoadActionClear);
    pRpd->colorAttachments()->object(0)->setClearColor(MTL::ClearColor::Make(1.0, .0, .0, 1.0));
    
    MTL::RenderCommandEncoder* pEnc = pCmdBuffer->renderCommandEncoder(pRpd);
    pEnc->endEncoding();
    pCmdBuffer->presentDrawable(_pDrawable);
    pCmdBuffer->commit();
    
    pRpd->release();
}

