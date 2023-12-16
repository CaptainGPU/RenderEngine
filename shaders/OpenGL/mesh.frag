#version 330

precision highp float;

in vec3 vertexColor;
out vec4 color;
uniform float u_time;

void main()
{
    vec3 colorSin = 0.5 + 0.5*cos(u_time + vertexColor * vec3(0.0, 2.0, 4.0));
    colorSin = vertexColor;
    color =  vec4(colorSin, 1.0f);
}  