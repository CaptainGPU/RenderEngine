#version 300 es

precision highp float;


out vec4 color;

void main()
{
    vec3 finalColor = vec3(1.0);

    color = vec4(finalColor, 1.0);
}  