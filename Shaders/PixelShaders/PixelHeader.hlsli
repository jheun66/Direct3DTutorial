Texture2D diffuseMap : register(t0);
Texture2D specularMap : register(t1);
Texture2D normalMap : register(t2);

SamplerState samp : register(s0);

cbuffer Light : register(b0)
{
    float3 lightDir;
    float specExp;
    float4 ambient;
    float4 ambientCeil;
}

cbuffer Material : register(b1)
{
    float4 mDiffuse;
    float4 mSpecular;
    float4 mAmbient;
    
    int4 hasMap;
}

struct Light
{
    float4 color;
    
    float3 position;
    float range;
    
    float3 direction;
    float outer;
    
    float inner;
    float length;
    int type;
    int active;
};
#define MAX_LIGHT 10

cbuffer LightInfo : register(b2)
{
    Light lights[MAX_LIGHT];
    uint lightCount;
}

float4 CalcAmbient(float3 normal, float4 color)
{
    float up = normal.y * 0.5f * 0.5f;
    
    float4 resultAmbient = ambient + up * ambientCeil;
    
    return resultAmbient * color;
}

float4 CalcDirection(float3 normal, float4 color, float3 worldPos, float3 camPos, Light light)
{
    float3 toLight = -normalize(light.direction);
    
    float diffuseIntensity = dot(toLight, normal);
    color *= light.color * saturate(diffuseIntensity) * mDiffuse;
    
    if (diffuseIntensity > 0.0f)
    {
        float3 toEye = normalize(camPos - worldPos);
        float3 halfWay = normalize(toEye + toLight);
        float specularIntensity = saturate(dot(halfWay, normal));
        color += light.color * pow(specularIntensity, specExp) * mSpecular;

    }
    
    return color;
}

float4 CalcPoint(float3 normal, float4 color, float3 worldPos, float3 camPos, Light light)
{
    float3 toLight = light.position - worldPos;
    float3 distanceToLight = length(toLight);
    toLight /= distanceToLight;
    
    float diffuseIntensity = dot(toLight, normal);
    color *= light.color * saturate(diffuseIntensity) * mDiffuse;
    
    if (diffuseIntensity > 0.0f)
    {
        float3 toEye = normalize(camPos - worldPos);
        float3 halfWay = normalize(toEye + toLight);
        float specularIntensity = saturate(dot(halfWay, normal));
        color += light.color * pow(specularIntensity, specExp) * mSpecular;
    }
        
    float distanceToLightNormal = 1.0f - saturate(distanceToLight / light.range);
    float attention = distanceToLightNormal * distanceToLightNormal;
 
    return color * attention;
}

float4 CalcSpot(float3 normal, float4 color, float3 worldPos, float3 camPos, Light light)
{
    float3 toLight = light.position - worldPos;
    float3 distanceToLight = length(toLight);
    toLight /= distanceToLight;
    
    float diffuseIntensity = dot(toLight, normal);
    color *= light.color * saturate(diffuseIntensity) * mDiffuse;
    
    if (diffuseIntensity > 0.0f)
    {
        float3 toEye = normalize(camPos - worldPos);
        float3 halfWay = normalize(toEye + toLight);
        float specularIntensity = saturate(dot(halfWay, normal));
        color += light.color * pow(specularIntensity, specExp) * mSpecular;
    }
        
    float3 dir = -normalize(light.direction);
    float cosAngle = dot(dir, toLight);
    
    float outer = cos(radians(light.outer));
    float inner = 1.0f / cos(radians(light.inner));
    
    // 번지는 값
    float conAttention = saturate((cosAngle - outer) * inner);
    conAttention *= conAttention;
    
    float distanceToLightNormal = 1.0f - saturate(distanceToLight / light.range);
    float attention = distanceToLightNormal * distanceToLightNormal;
 
    return color * attention * conAttention;
}

float4 CalcCapsule(float3 normal, float4 color, float3 worldPos, float3 camPos, Light light)
{
    float3 direction = normalize(light.direction);
    float3 start = worldPos - light.position;
    float distanceOnLine = dot(start, direction) / light.length;
    distanceOnLine = saturate(distanceOnLine) * light.length;
    
    float3 pointOnLine = light.position + direction * distanceOnLine;
    float3 toLight = pointOnLine - worldPos;
    float3 distanceToLight = length(toLight);
    toLight /= distanceToLight;
    
    float diffuseIntensity = dot(toLight, normal);
    color *= light.color * saturate(diffuseIntensity) * mDiffuse;
    
    if (diffuseIntensity > 0.0f)
    {
        float3 toEye = normalize(camPos - worldPos);
        float3 halfWay = normalize(toEye + toLight);
        float specularIntensity = saturate(dot(halfWay, normal));
        color += light.color * pow(specularIntensity, specExp) * mSpecular;
    }
        
    float distanceToLightNormal = 1.0f - saturate(distanceToLight / light.range);
    float attention = distanceToLightNormal * distanceToLightNormal;
 
    return color * attention;
}