#pragma once

#include <cstdint>

class Window
{
public:
    Window(uint32_t width, uint32_t height);
    virtual ~Window() = default;
    
    virtual void frame();
    virtual void close();
    virtual bool isShouldClose();
    
protected:
    uint32_t mWidth = 0;
    uint32_t mHeight = 0;
};

