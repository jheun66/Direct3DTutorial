Texture2D diffuseMap : register(t0);
Texture2D specularMap : register(t1);
Texture2D normalMap : register(t2);

SamplerState samp : register(s0);

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

cbuffer LightInfo : register(b0)
{
    Light lights[MAX_LIGHT];
    uint lightCount;
    
    float3 padding;
    
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

float NormalMapping(float3 tangent, float3 binormal, float3 normal, float3 lightDir, float2 uv)
{
    float3 light = normalize(lightDir);
    
                                   // polygon의 
    tangent = normalize(tangent); // x
    binormal = normalize(binormal); // y
    normal = normalize(normal); // z
    
    if (hasMap[2])
    {
        float4 normalMapping = normalMap.Sample(samp, uv);
    
        float3x3 TBN = float3x3(tangent, binormal, normal);
    
        // color 범위 0 ~ 1 을 벡터의 -1 ~ 1 로 바꿔주기 위해
        normal = normalMapping.xyz * 2.0f - 1.0f;
        normal = normalize(mul(normal, TBN));
    }
    
    return saturate(dot(normal, light));
}

float4 SpecularMapping(float3 normal, float3 camPos, float3 lightDir, 
                    float3 worldPos, float2 uv)
{
    float3 toEye = normalize(camPos - worldPos);
    float3 halfWay = normalize(toEye + lightDir);
    float specularIntensity = saturate(dot(halfWay, normal));
    
    specularIntensity = pow(specularIntensity, mSpecular.w);
    
    if(hasMap[1])
    {
        float4 specularMapping = specularMap.Sample(samp, uv);
        return specularMapping * specularIntensity;
    }
    
    return float4(specularIntensity.xxx, 1.0f) * mSpecular;
}

float4 CalcAmbient(float3 normal, float4 color)
{
    float up = normal.y * 0.5f * 0.5f;
    
    float4 resultAmbient = ambient + up * ambientCeil;
    
    return resultAmbient * color;
}

float4 CalcDirection(float3 tangent, float3 binoraml, float3 normal, 
                float4 color, float3 worldPos, float3 camPos, Light light, float2 uv)
{
    float3 toLight = -normalize(light.direction);
    
    float diffuseIntensity = NormalMapping(tangent, binoraml, normal, toLight, uv);
    color *= light.color * saturate(diffuseIntensity) * mDiffuse;
    
    if (diffuseIntensity > 0.0f)
    {
        color += light.color * SpecularMapping(normal, camPos, toLight, worldPos, uv);
    }
    
    return float4(color.rgb, 1.0f);
}

float4 CalcPoint(float3 tangent, float3 binoraml, float3 normal, 
            float4 color, float3 worldPos, float3 camPos, Light light, float2 uv)
{
    float3 toLight = light.position - worldPos;
    float3 distanceToLight = length(toLight);
    toLight /= distanceToLight;
    
    float diffuseIntensity = NormalMapping(tangent, binoraml, normal, toLight, uv);
    color *= light.color * saturate(diffuseIntensity) * mDiffuse;
    
    if (diffuseIntensity > 0.0f)
    {
        color += light.color * SpecularMapping(normal, camPos, toLight, worldPos, uv);
    }
        
    float distanceToLightNormal = 1.0f - saturate(distanceToLight / light.range);
    float attention = distanceToLightNormal * distanceToLightNormal;
 
    return float4(color.rgb * attention, 1.0f);
}

float4 CalcSpot(float3 tangent, float3 binoraml, float3 normal,
            float4 color, float3 worldPos, float3 camPos, Light light, float2 uv)
{
    float3 toLight = light.position - worldPos;
    float3 distanceToLight = length(toLight);
    toLight /= distanceToLight;
    
    float diffuseIntensity = NormalMapping(tangent, binoraml, normal, toLight, uv);
    color *= light.color * saturate(diffuseIntensity) * mDiffuse;
    
    if (diffuseIntensity > 0.0f)
    {
        color += light.color * SpecularMapping(normal, camPos, toLight, worldPos, uv);
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
 
    return float4(color.rgb * attention * conAttention, 1.0f);
}

float4 CalcCapsule(float3 tangent, float3 binoraml, float3 normal,
            float4 color, float3 worldPos, float3 camPos, Light light, float2 uv)
{
    float3 direction = normalize(light.direction);
    float3 start = worldPos - light.position;
    float distanceOnLine = dot(start, direction) / light.length;
    distanceOnLine = saturate(distanceOnLine) * light.length;
    
    float3 pointOnLine = light.position + direction * distanceOnLine;
    float3 toLight = pointOnLine - worldPos;
    float3 distanceToLight = length(toLight);
    toLight /= distanceToLight;
    
    float diffuseIntensity = NormalMapping(tangent, binoraml, normal, toLight, uv);
    color *= light.color * saturate(diffuseIntensity) * mDiffuse;
    
    if (diffuseIntensity > 0.0f)
    {
        color += light.color * SpecularMapping(normal, camPos, toLight, worldPos, uv);
    }
        
    float distanceToLightNormal = 1.0f - saturate(distanceToLight / light.range);
    float attention = distanceToLightNormal * distanceToLightNormal;
 
    return float4(color.rgb * attention, 1.0f);
}