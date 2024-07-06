#version 300 es

precision highp float;

in vec2 v_texcoord;

out vec4 color;

uniform sampler2D u_texture_0;
uniform sampler2D u_texture_1;
uniform int u_blurRange;

void main()
{
	int n = 0;
	vec2 texelSize = 1.0 / vec2(textureSize(u_texture_0, 0));
	float result = 0.0;

    float outFragColor = 1.0;

    if (u_blurRange > 0)
    {
        for (int x = -u_blurRange; x <= u_blurRange; x++) 
        {
            for (int y = -u_blurRange; y <= u_blurRange; y++) 
            {
                vec2 offset = vec2(float(x), float(y)) * texelSize;
                result += texture(u_texture_0, v_texcoord + offset).r;
                n++;
            }
        }

        outFragColor = result / (float(n));
    }
    else
    {
        outFragColor = texture(u_texture_0, v_texcoord).r;
    }

	


    vec3 worldColor = texture(u_texture_1, v_texcoord).xyz;
    worldColor *= outFragColor;

    color = vec4(worldColor, 1.0);
}