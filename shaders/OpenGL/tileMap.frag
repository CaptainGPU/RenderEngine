#version 330

layout (location = 0) out vec4 gColor;
in vec2 v_palleteUV;

uniform sampler2D u_palleteTexture;

void main()
{
    vec2 paletteUV = v_palleteUV;
    vec4 palleteColor = texture(u_palleteTexture, paletteUV);

    gColor = palleteColor;
}