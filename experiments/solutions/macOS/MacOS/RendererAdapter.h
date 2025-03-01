//
//  RendererAdapter.h
//  MacOS
//
//  Created by Eugene Karpenko on 26.11.2024.
//

#import "Metal/MTLDevice.h"
#import "QuartzCore/CAMetalLayer.h"
#import "Renderer.hpp"

@interface RendererAdapter : NSObject
{
    Renderer* _pRenderer;
}

-(void)draw:(id <CAMetalDrawable>) drawable device: (id <MTLDevice>) device;
@end
