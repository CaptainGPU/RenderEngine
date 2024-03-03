#version 330

in vec2 v_texcoord;

uniform sampler2D u_texture_0;
uniform sampler2D u_texture_1;
uniform sampler2D u_texture_2;
uniform mat4 u_projectionMatrix;
uniform float u_aoRadius;
uniform float u_aoBias;
uniform int u_aoKernels;

uniform vec3 u_samples[64];

uniform vec4 View_InvDeviceZToWorldZTransform;

layout (location = 0) out vec4 color;

const float PI = 3.14159265359;

// Function to generate random rotations in the range [0, 2*PI)
vec2 randomRotations(float seed) {
  float angle = 2.0 * PI * fract(sin(seed) * 43758.5453123);
  return vec2(cos(angle), sin(angle));
}


float ConvertFromDeviceZ(float DeviceZ)
{
	return DeviceZ * View_InvDeviceZToWorldZTransform[0] + View_InvDeviceZToWorldZTransform[1] + 1.0f / (DeviceZ * View_InvDeviceZToWorldZTransform[2] - View_InvDeviceZToWorldZTransform[3]);
}


float GetDeviceZFromAOInput(vec2 textureUV)
{
    float z = texture(u_texture_0, textureUV).r;
    return 1.0 - z;
}

float GetSceneDepthFromAOInput(vec2 TextureUV)
{
    float zAO = GetDeviceZFromAOInput(TextureUV);
    return zAO;
	//return ConvertFromDeviceZ(GetDeviceZFromAOInput(TextureUV));
}

float CalculateGTAO(vec2 textureUV)
{
    vec4 DepthBufferSizeAndInvSize = vec4(800.0, 600.0, 1.0 / 800.0f, 1.0 / 600.0f);

    float SceneDepth = GetSceneDepthFromAOInput(textureUV);

    textureUV += DepthBufferSizeAndInvSize.zw * 0.125;

    return SceneDepth;
}

float near = 0.1; 
float far  = 100.0; 

float LinearizeDepth(float depth) 
{
    float z = depth * 2.0 - 1.0; // back to NDC 
    return (2.0 * near * far) / (far + near - z * (far - near));	
}

vec3 screenToViewSpace(vec2 uv, float SceneDepth)
{
    float x = uv.x;
    float y = uv.y;

    float normalizedX = (x * 2.0) - 1.0;
    float normalizedY = (y * 2.0) - 1.0;;

    vec4 clipSpace = vec4(normalizedX, normalizedY, SceneDepth, 1.0f);

    vec4 viewSpacePos = u_projectionMatrix * clipSpace;

    return viewSpacePos.xyz;
}

const vec2 noiseScale = vec2(800.0/4.0, 600.0/4.0);

void main()
{
    vec2 texCoord = v_texcoord;
    
    // float SceneDepth = texture(u_texture_0, texCoord).x;

    // float x = texCoord.x * 2.0 - 1.0;
    // float y = texCoord.y * 2.0 - 1.0;
    // float z = SceneDepth * 2.0 - 1.0;

    // mat4 inverseMat = inverse(u_projectionMatrix);
    // vec4 viewPosH = inverseMat * vec4(x, y, z, 1.0);
    // vec3 viewPos = viewPosH.xyz / viewPosH.w;
    // viewPos.z = abs(viewPos.z);

    // vec3 ViewDir = -normalize(viewPos.xyz);
    // vec3 finalColor = ViewDir;

    //viewPos.z = abs(viewPos.z);

    // float valZ = viewPos.z;

    // float valX = viewPos.x;
    // float valY = viewPos.y;

    // vec3 finalColor = vec3(0.0);

    // if (valX > 0.0)
    // {
    //     finalColor.x = valX / 50.0;
    // }

    // if (valY > 0.0)
    // {
    //     finalColor.y = valY / 50.0;
    // }



    // if (valZ > 0.0)
    // {
    //     finalColor.z = valZ / 50.0f;
    // }

    
    // vec3 fragPos = texture(u_texture_0, texCoord).xyz;
    // vec3 normal = texture(u_texture_1, texCoord).xyz;
    // vec3 randomVec = texture(u_texture_2, texCoord * noiseScale).xyz; 

    // vec3 tangent   = normalize(randomVec - normal * dot(randomVec, normal));
    // vec3 bitangent = cross(normal, tangent);
    // mat3 TBN       = mat3(tangent, bitangent, normal);  

    // float occlusion = 0.0;
    // for(int i = 0; i < kernelSize; ++i)
    // {
    //     // get sample position
    //     vec3 samplePos = TBN * samples[i]; // from tangent to view-space
    //     samplePos = fragPos + samplePos * radius; 
        
    //     // project sample position (to sample texture) (to get position on screen/texture)
    //     vec4 offset = vec4(samplePos, 1.0);
    //     offset = u_projectionMatrix * offset; // from view to clip-space
    //     offset.xyz /= offset.w; // perspective divide
    //     offset.xyz = offset.xyz * 0.5 + 0.5; // transform to range 0.0 - 1.0
        
    //     // get sample depth
    //     float sampleDepth = texture(u_texture_0, offset.xy).z; // get depth value of kernel sample
        
    //     // range check & accumulate
    //     float rangeCheck = smoothstep(0.0, 1.0, radius / abs(fragPos.z - sampleDepth));
    //     occlusion += (sampleDepth >= samplePos.z + bias ? 1.0 : 0.0) * rangeCheck; 
    // }

    // occlusion = 1.0 - (occlusion / kernelSize);

    // vec3 finalColor = vec3(tangent);

    // color = vec4(finalColor, 1.0);

    vec3 fragPos = texture(u_texture_0, texCoord).rgb;
	vec3 normal = normalize(texture(u_texture_1, texCoord).rgb * 2.0 - 1.0);

    // Get a random vector using a noise lookup
	ivec2 texDim = textureSize(u_texture_0, 0); 
	ivec2 noiseDim = textureSize(u_texture_2, 0);

    float a = float(texDim.x);
    float b = float(noiseDim.x);
    float d = float(texDim.y);
    float c = float(noiseDim.y);

    vec2 noiseUV = vec2(a / b, d/c) * texCoord;
    vec3 randomVec = texture(u_texture_2, noiseUV).xyz * 2.0 - 1.0;

    // Create TBN matrix
	vec3 tangent = normalize(randomVec - normal * dot(randomVec, normal));
	vec3 bitangent = cross(tangent, normal);
	mat3 TBN = mat3(tangent, bitangent, normal);

    // Calculate occlusion value
	float occlusion = 0.0f;
	// remove banding
    for(int i = 0; i < u_aoKernels; i++)
	{		
		vec3 samplePos = TBN * u_samples[i].xyz; 
		samplePos = fragPos + samplePos * u_aoRadius; 
		
		// project
		vec4 offset = vec4(samplePos, 1.0f);
		offset = u_projectionMatrix * offset; 
		offset.xyz /= offset.w; 
		offset.xyz = offset.xyz * 0.5f + 0.5f; 
		
		float sampleDepth = -texture(u_texture_0, offset.xy).w; 

		float rangeCheck = smoothstep(0.0f, 1.0f, u_aoRadius / abs(fragPos.z - sampleDepth));
		occlusion += (sampleDepth >= samplePos.z + u_aoBias ? 1.0f : 0.0f) * rangeCheck;           
	}
	occlusion = 1.0 - (occlusion / float(u_aoKernels));

    vec3 fColor = vec3(occlusion);

    color = vec4(fColor, 1.0);
}