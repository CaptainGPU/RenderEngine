//
// Created by Eugene Karpenko @ CaptainGPU
// https://twitter.com/CaptainGPU
//

#pragma once

class Texture;

class FrameBuffer
{
public:
    FrameBuffer();
    
    void setColorTexture(Texture* texture);
    Texture* getColorTexture();

    void setDepthTexture(Texture* texture);
    Texture* getDepthTexture();
    
    unsigned int get_openGL_FBO();
    void set_openGL_FBO(unsigned int fbo);
    
private:
    unsigned int m_openGL_FBO;
    Texture* m_colorTexture;
    Texture* m_depthTexture;
};
