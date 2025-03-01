//
//  ViewController.m
//  MacOS
//
//  Created by Eugene Karpenko on 26.11.2024.
//

#import "ViewController.h"
#import "RendererAdapter.h"

@implementation ViewController
{
    MTKView* _view;
    RendererAdapter* _pRendererAdapter;
}

- (void)viewDidLoad {
    [super viewDidLoad];
    
    _view = (MTKView*)self.view;
    _pRendererAdapter = [RendererAdapter alloc];
    _view.device = MTLCreateSystemDefaultDevice();
    [_pRendererAdapter draw:_view.currentDrawable device:_view.device];

    // Do any additional setup after loading the view.
}


- (void)setRepresentedObject:(id)representedObject {
    [super setRepresentedObject:representedObject];

    // Update the view, if already loaded.
}


@end
