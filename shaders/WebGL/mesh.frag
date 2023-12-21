#version 300 es

precision highp float;

in vec3 v_normal;
in vec3 vertexColor;

out vec4 color;

uniform float u_time;

void main()
{
    vec3 colorSin = 0.5 + 0.5*cos(u_time + vertexColor * vec3(0.0, 2.0, 4.0));
    colorSin = v_normal;
    color =  vec4(colorSin, 1.0f);
}  