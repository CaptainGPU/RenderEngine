//
//  Renderer.hpp
//  MacOS
//
//  Created by Eugene Karpenko on 26.11.2024.
//

#include "Metal/Metal.hpp"
#include "QuartzCore/CAMetalDrawable.hpp"

class Renderer
{
public:
    Renderer(CA::MetalDrawable* const pDrawable, MTL::Device* const pDevice);
    ~Renderer();
    void draw() const;
    
private:
    CA::MetalDrawable* _pDrawable = nullptr;
    MTL::Device* _pDevice = nullptr;
    MTL::CommandQueue* _pCommandQueue = nullptr;
};
