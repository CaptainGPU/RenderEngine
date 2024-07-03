//
//  openGLGraphicsDevice.h
//  RenderEngine
//
//  Created by Eugene Karpenko on 11.06.2024.
//

#pragma once

#include "graphicsDevice.h"

class OpenGLGraphicsDevice : public GraphicsDevice
{
public:
    OpenGLGraphicsDevice();
    virtual std::string getName() override;
};
