#version 330

layout (location = 0) in vec3 a_position;
layout (location = 1) in vec2 a_textcoord;
layout (location = 2) in vec3 a_normal;
layout (location = 3) in vec3 a_color;

uniform mat4 u_modelMatrix;
uniform mat4 u_viewMatrix;
uniform mat4 u_projectionMatrix;
uniform float u_cellPalleteIndex[567];
uniform float u_palleteWidth;
uniform float u_palleteHeight;

out vec2 v_palleteUV;

void main()
{
    int cellIndex = int(a_normal.x);
    float cellIndexInPallete = u_cellPalleteIndex[cellIndex];

    vec2 palleteCellSize = vec2(1.0) / vec2(u_palleteWidth, u_palleteHeight);

    float pY = floor(cellIndexInPallete/u_palleteWidth);
    float pX = mod(cellIndexInPallete, u_palleteWidth);

    float sX = pX * palleteCellSize.x;
    float sY = pY * palleteCellSize.y;

    v_palleteUV = vec2(sX, sY) + (a_textcoord * palleteCellSize); //a_textcoord * palleteCellSize;

    vec3 position = a_position;
    int test = int(cellIndexInPallete);

    if (test == 0)
    {
        position = vec3(0.0);
    }

    gl_Position = u_projectionMatrix * u_viewMatrix * u_modelMatrix * vec4(position, 1.0f);
}