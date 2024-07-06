#version 300 es

precision highp float;

const float PI = 3.14159265359;

struct PointLight
{
    vec3 position;
    vec3 color;
    float radius;
};

struct SpotLight
{
    vec3 direction;
    vec3 color;
    vec3 position;
    float invRange;

    float cutOff;
    float outerCutOff;
};

const int MAX_POINT_LIGHTS = 4;
const int MAX_SPOT_LIGHTS = 4;

in vec3 v_normal;
in vec3 v_position;
in vec3 vertexColor;
in vec4 v_fragPosLightSpace;
in float v_clipSpacePosition;
in vec4 v_fragPosSpotLightSpace[MAX_SPOT_LIGHTS];

out vec4 color;
uniform float u_time;
uniform float u_gamma;
uniform vec3 u_materialBaseColor;
uniform vec3 u_lightColor;
uniform vec3 u_ambientColor;
uniform float u_smoothness;
uniform float u_ambientStrength;
uniform float u_specularStrength;
uniform vec3 u_cameraPosition;
uniform PointLight u_pointLights[MAX_POINT_LIGHTS];
uniform SpotLight u_spotLights[MAX_SPOT_LIGHTS];
uniform int u_pointLightsCount;
uniform int u_spotLightsCount;
uniform vec3 u_sunDirection;
uniform float u_sunItensity;
uniform sampler2D u_texture_0;
uniform float u_shadowDistance;
uniform int u_spotLightShadowCount;
uniform int u_pointLightShadowCount;
// spots shadowmaps
uniform sampler2D u_texture_1;
uniform sampler2D u_texture_2;
uniform sampler2D u_texture_3;
uniform sampler2D u_texture_4;

uniform samplerCube u_texture_5;
uniform samplerCube u_texture_6;
uniform samplerCube u_texture_7;
uniform samplerCube u_texture_8;

float selectPointLightDep(int index, vec3 uv)
{
    float depth = 1.0;
    
    if(index == 0) 
    {
        depth = texture(u_texture_5, uv).r;
    }

    if(index == 1) 
    {
        depth = texture(u_texture_6, uv).r;
    }

    if(index == 2) 
    {
        depth = texture(u_texture_7, uv).r;
    }

    if(index == 3) 
    {
        depth = texture(u_texture_8, uv).r;
    }

    return depth;
}

float calculatePointLightShadow(int index)
{
    vec3 lightPos = u_pointLights[index].position;
    vec3 toLight = v_position - lightPos;
    vec3 dir = normalize(toLight);

    float radius = u_pointLights[index].radius;
    
    float closestDepth = selectPointLightDep(index, dir);
    vec3 color = texture(u_texture_5, dir).rgb;

    float currentDepth = length(toLight);
    currentDepth /= radius;

    float bias = 0.01; 
    float shadow = currentDepth -  bias > closestDepth ? 0.0 : 1.0;

    return shadow;
}

vec3 calculatePointLight(int index, vec3 normal, vec3 viewDir, vec3 ambientColor)
{
    vec3 lightColor = pow(u_pointLights[index].color, vec3(u_gamma));
    vec3 lightPosition = u_pointLights[index].position;
    vec3 lightDir = normalize(lightPosition - v_position);
    vec3 halfwayDir = normalize(lightDir + viewDir);  

    float diff = max(dot(normal, lightDir), 0.0);

    float specPow = mix(2.0, 512.0, u_smoothness);

    float spec = pow(max(dot(normal, halfwayDir), 0.0), specPow);
    spec *= u_specularStrength;

    float distance = length(lightPosition - v_position);

    float range = 10.0;

    float constant = 1.0f;
    float linear = 4.5 / range;
    float quadratic = 75.0f/(range*range);

    float attenuation = 1.0 / (constant + linear * distance + quadratic * (distance * distance));
    attenuation = clamp(attenuation, .0, 1.0);

    vec3 ambient = u_ambientStrength * lightColor;
    vec3 diffuse = diff * lightColor;
    vec3 specular = spec * lightColor;

    vec3 ToLight = lightPosition - v_position;
    float DistanceSqr = dot(ToLight, ToLight);

    attenuation = 1.0 / (DistanceSqr + 1.0);

    float InvRadius = 1.0 / range;

    float A = (InvRadius * InvRadius);
    float B = DistanceSqr * A;
    float C = B * B;
    float D = 1.0 - C;
    float E = clamp(D, 0.0, 1.0);
    float F = E * E;

    attenuation *= F;

    //float LightRadiusMask = Square(E);

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    float shadow = 1.0;
    
    if (index < u_pointLightShadowCount)
    {
        shadow = calculatePointLightShadow(index);
    }

    return ambient + shadow * (diffuse + specular);

    //return vec3(attenuation);
}

float sampleSpotLightShadowMask(int index, vec2 uv)
{
    float mask = 1.0;

    if (index == 0)
    {
        mask = texture(u_texture_1, uv).x;
    }

    if (index == 1)
    {
        mask = texture(u_texture_2, uv).x;
    }

     if (index == 2)
    {
        mask = texture(u_texture_3, uv).x;
    }

     if (index == 3)
    {
        mask = texture(u_texture_4, uv).x;
    }

    return mask;
}

float calculateSpotLightShadow(int index, vec3 normal, vec3 lightDir)
{
    vec3 projCoords = v_fragPosSpotLightSpace[index].xyz / v_fragPosSpotLightSpace[index].w;
    vec2 uvCoords;
    uvCoords.x = 0.5 * projCoords.x + 0.5;
    uvCoords.y = 0.5 * projCoords.y + 0.5;
    float z = 0.5 * projCoords.z + 0.5;

    if (z > 1.0)
    {
        return 1.0;
    }

    float depth = sampleSpotLightShadowMask(index, uvCoords);

    float factor = dot(normal, lightDir);
    factor = clamp(factor, 0.0, 1.0);

    float bias = 0.0005;

    float shadow = (depth + bias) < z ? 0.0 : 1.0;
        
    return shadow;
}

vec3 spotLightShadow(int index)
{
    vec3 projCoords = v_fragPosSpotLightSpace[index].xyz / v_fragPosSpotLightSpace[index].w;
    vec2 uvCoords;
    uvCoords.x = 0.5 * projCoords.x + 0.5;
    uvCoords.y = 0.5 * projCoords.y + 0.5;
    float z = 0.5 * projCoords.z + 0.5;

    if (z > 1.0)
    {
        return vec3(1.0);
    }

    float depth = sampleSpotLightShadowMask(index, uvCoords);

    float bias = 0.0025;

    float shadow = (depth + bias) < z ? 0.0 : 1.0;
        
    return vec3(shadow);
}

vec3 calculateSpotLight(int index, vec3 normal, vec3 viewDir)
{
    vec3 lightPosition = u_spotLights[index].position;
    vec3 lightDirection = u_spotLights[index].direction;
    vec3 ToLight = lightPosition - v_position;

    vec3 ToLightN = normalize(ToLight);

    float theta = dot(ToLightN, -lightDirection);

    // float cutOff = cos(0.436332313);
    // float outerCutOff = cos(0.523598776);

    float cutOff = u_spotLights[index].cutOff;
    float outerCutOff = u_spotLights[index].outerCutOff;

    float epsilon = cutOff - outerCutOff;
    float intensity = clamp((theta - outerCutOff) / epsilon, 0.0, 1.0);

    float col = theta > cutOff ? 1.0 : 0.0;

    float DistanceSqr = dot(ToLight, ToLight);

    float attenuation = 1.0 / (DistanceSqr + 1.0);

    float InvRadius = u_spotLights[index].invRange;

    float A = (InvRadius * InvRadius);
    float B = DistanceSqr * A;
    float C = B * B;
    float D = 1.0 - C;
    float E = clamp(D, 0.0, 1.0);
    float F = E * E;

    attenuation = F;

    float shadow = 1.0;
    
    if (index < u_spotLightShadowCount)
    {
        shadow = calculateSpotLightShadow(index, normal, ToLightN);
    }

    float diff = max(dot(normal, ToLightN), 0.0);
    vec3 reflectDir = reflect(-ToLightN, normal);
    vec3 lightColor = pow(u_spotLights[index].color, vec3(u_gamma));; 
    vec3 halfwayDir = normalize(ToLightN + viewDir);  

    float specPow = mix(2.0, 512.0, u_smoothness);

    float spec = pow(max(dot(normal, halfwayDir), 0.0), specPow);
    spec *= u_specularStrength;


    vec3 ambient = u_ambientStrength * lightColor;
    vec3 diffuse = diff * lightColor;
    vec3 specular = spec * lightColor;

    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;


    vec3 finalColor = ambient + shadow * (diffuse + specular);
    
    return finalColor;
}

float calculateShadow()
{
    vec3 projCoords = v_fragPosLightSpace.xyz / v_fragPosLightSpace.w;
    vec2 uvCoords;
    uvCoords.x = 0.5 * projCoords.x + 0.5;
    uvCoords.y = 0.5 * projCoords.y + 0.5;
    float z = 0.5 * projCoords.z + 0.5;

    if (z > 1.0)
    {
        return 1.0;
    }

    float depth = texture(u_texture_0, uvCoords).x;

    float bias = 0.0025;

    float shadow = (depth + bias) < z ? 0.0 : 1.0;
        
    return shadow;
}

vec3 calculateDirectionLight(vec3 normal, vec3 viewDir)
{
    vec3 lightColor = pow(u_lightColor, vec3(u_gamma));
    vec3 lightDirection = u_sunDirection;
    vec3 lightDir = normalize(-lightDirection);
    float diff = max(dot(normal, lightDir), 0.0);

    vec3 halfwayDir = normalize(lightDir + viewDir);  
    
    float specPow = mix(2.0, 512.0, u_smoothness);

    float spec = pow(max(dot(normal, halfwayDir), 0.0), specPow);
    spec *= u_specularStrength;

    vec3 ambient = u_ambientStrength * lightColor;
    vec3 diffuse = diff * lightColor;
    vec3 specular = spec * lightColor;

    float shadow = calculateShadow();

    shadow = v_clipSpacePosition < u_shadowDistance ? shadow : 1.0;

    vec3 finalColor = ambient + shadow * (diffuse + specular);
    finalColor *= u_sunItensity;

    return finalColor;
}

void main()
{
    vec3 lighing  = vec3(0);

    vec3 normal = normalize(v_normal);
    vec3 viewDir = normalize(u_cameraPosition - v_position);

    lighing += calculateDirectionLight(normal, viewDir);

    for (int i = 0; i < u_pointLightsCount; i++)
    {
        lighing += calculatePointLight(i, normal, viewDir, u_ambientColor);
    }

    for (int i = 0; i < u_spotLightsCount; i++)
    {
        lighing += calculateSpotLight(i, normal, viewDir);
    }

    vec3 finalColor = lighing * u_materialBaseColor;

    color = vec4(finalColor, 1.0);
}  