#version 300 es

precision highp float;

out vec4 color;
in float v_cellIndex;
in float v_test;

/*in vec3 v_worldPos;
in vec3 v_worldNormal;
in vec3 v_vertexColor;



float farPlane = 100.0f;
float nearPlane = 0.1f;

float linearDepth(float depth)
{
	float z = depth * 2.0f - 1.0f; 
	return (2.0f * nearPlane * farPlane) / (farPlane + nearPlane - z * (farPlane - nearPlane));	
}*/

void main()
{
    vec3 colorCell = vec3(1.0);
    
    if (v_test == 1.0)
    {
        //colorCell = vec3(0.454, 0.827, 0.819);
        colorCell = vec3(0.243, 0.596, 0.439);
    }

    if (v_test == 2.0)
    {
        colorCell = vec3(0.141, 0.258, 0.592);
    }

    if (v_test == 3.0)
    {
        colorCell = vec3(0.650, 0.266, 0.219);
    }
    
    //gPosition = vec4(v_worldPos, linearDepth(gl_FragCoord.z));
    //gNormal = vec4(normalize(v_worldNormal) * 0.5 + 0.5, 1.0);
    color = vec4(colorCell, 1.0);
}