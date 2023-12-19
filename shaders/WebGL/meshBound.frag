#version 300 es

precision highp float;

in vec3 boundColor;
out vec4 color;

void main()
{
    color = vec4(boundColor, 1.0);
}  