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

    void setColorTexture1(Texture* texture);
    Texture* getColorTexture1();

    void setColorTexture2(Texture* texture);
    Texture* getColorTexture2();

    void setDepthTexture(Texture* texture);
    Texture* getDepthTexture();
    
    unsigned int get_openGL_FBO();
    void set_openGL_FBO(unsigned int fbo);

    void setCubeTexture(Texture* texture);
    Texture* getCubeTexture();

    void setCubeDepthTexture(Texture* texture);
    Texture* getCubeDepthTexture();

    void setAttachmentCount(unsigned int count);
    unsigned int getAttachmentCount();
    
private:
    unsigned int m_attacmhentCount;
    unsigned int m_openGL_FBO;
    Texture* m_colorTexture;
    Texture* m_colorTexture1;
    Texture* m_colorTexture2;
    Texture* m_depthTexture;
    Texture* m_cubeMapTexture;
    Texture* m_cubeDepthTexture;
};
