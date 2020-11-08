static const float2 specPowerRange = { 0.1f, 250.0f };

struct Material
{
    float3 normal;
    float4 diffuseColor;
    float specPow;
    float specIntensity;
};

struct SurfaceData
{
    float linearDepth;
    float3 color;
    float3 normal;
    float specInt;
    float specPow;
};

Texture2D diffuseMap : register(t0);
Texture2D specularMap : register(t1);
Texture2D normalMap : register(t2);

Texture2D depthTexture : register(t3);
Texture2D colorSpecIntTexture : register(t4);
Texture2D normalTexture : register(t5);
Texture2D specPowTexture : register(t6);

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

cbuffer MaterialBuffer : register(b1)
{
    float4 mDiffuse;
    float4 mSpecular;
    float4 mAmbient;
    float4 mEmissive;
    
    int4 hasMap;
}

cbuffer ViewBuffer : register(b2)
{
    matrix view;
    matrix viewInv;
}

cbuffer ProjectionBuffer : register(b3)
{
    matrix perspective;
}

float NormalMapping(float3 tangent, float3 binormal, float3 normal, float3 lightDir, float2 uv)
{
    float3 light = normalize(lightDir);
    
    tangent = normalize(tangent);
    binormal = normalize(binormal);
    normal = normalize(normal);
        
    if (hasMap[2])
    {
        float4 normalMapping = normalMap.Sample(samp, uv);
    
        float3x3 TBN = float3x3(tangent, binormal, normal);
        normal = normalMapping.xyz * 2.0f - 1.0f;
        normal = normalize(mul(normal, TBN));
    }
    
    return saturate(dot(normal, light));
}

float3 GetMappingNormal(float3 tangent, float3 binormal, float3 normal, float2 uv)
{
    tangent = normalize(tangent);
    binormal = normalize(binormal);
    normal = normalize(normal);
        
    if (hasMap[2])
    {
        float4 normalMapping = normalMap.Sample(samp, uv);
    
        float3x3 TBN = float3x3(tangent, binormal, normal);
        normal = normalMapping.xyz * 2.0f - 1.0f;
        normal = normalize(mul(normal, TBN));
    }
    
    return normal;
}
/*
void NormalMapping(float2 uv, float3 normal, float3 tangent)
{
    float4 map = normalMap.Sample(samp, uv);
    
    [flatten]
    if(any(map.rgb) == false)
        return;
    
    float3 N = normalize(normal);
    
    [flatten]
    if (any(tangent) == false)
        tangent = float3(1e-6f, 1e-6f, 1e-6f);

    float3 T = normalize(tangent - dot(tangent, N) * N);
    float3 B = normalize(cross(N, T));
    float3x3 TBN = float3x3(T, B, N);

    float3 coord = map.rgb * 2.0f - 1.0f;
    
    coord = mul(coord, TBN);
    
    mDiffuse *= saturate(dot(-lights[0].direction, coord));
}*/

float4 SpecularMapping(float3 normal, float3 camPos, float3 lightDir,
float3 worldPos, float2 uv)
{
    float3 toEye = normalize(camPos - worldPos);
    float3 halfWay = normalize(toEye + lightDir);
    float specularIntensity = saturate(dot(halfWay, normal));
    
    specularIntensity = pow(specularIntensity, mSpecular.w);
    
    if (hasMap[1])
    {
        float4 specularMapping = specularMap.Sample(samp, uv);
        return specularMapping * specularIntensity;
    }
    
    return float4(specularIntensity.xxx, 1.0f) * mSpecular;
}

float4 CalcAmbient(float3 normal, float4 color)
{
    float up = normal.y * 0.5f + 0.5f;
    
    float4 resultAmbient = ambient + up * ambientCeil;
    
    return resultAmbient * color;
}

float4 CalcDirection(float3 tangent, float3 binormal, float3 normal,
    float4 color, float3 worldPos, float3 camPos, Light light, float2 uv)
{
    float3 toLight = -normalize(light.direction);
    
    float diffuseIntensity = NormalMapping(tangent, binormal, normal, toLight, uv);
    color *= light.color * saturate(diffuseIntensity) * mDiffuse;
    
    if (diffuseIntensity > 0.0f)
    {
        color += light.color * SpecularMapping(normal, camPos, toLight, worldPos, uv);
    }
    
    return float4(color.rgb, 1.0f);
}

float4 CalcDirection(float3 position, Material mat, Light light)
{
    float3 toLight = -normalize(light.direction);
    
    float NDotL = dot(toLight, mat.normal);
    float3 finalColor = light.color.rgb * saturate(NDotL);
    
    float3 camPos = viewInv._41_42_43;
    float3 toEye = normalize(camPos - position);
    float3 halfWay = normalize(toEye - toLight);
    float NDotH = saturate(dot(halfWay, mat.normal));
    finalColor += light.color.rgb * pow(NDotH, mat.specPow) * mat.specIntensity;
    
    return float4(finalColor * mat.diffuseColor.rgb, 1.0f);
}

float4 CalcPoint(float3 tangent, float3 binormal, float3 normal, float4 color,
    float3 worldPos, float3 camPos, Light light, float2 uv)
{
    float3 toLight = light.position - worldPos;
    float distanceToLight = length(toLight);
    toLight /= distanceToLight;
    
    float diffuseIntensity = NormalMapping(tangent, binormal, normal, toLight, uv);
    color *= light.color * saturate(diffuseIntensity) * mDiffuse;
    
    if (diffuseIntensity > 0.0f)
    {
        color += light.color * SpecularMapping(normal, camPos, toLight, worldPos, uv);
    }
    
    float distanceToLightNormal = 1.0f - saturate(distanceToLight / light.range);
    float attention = distanceToLightNormal * distanceToLightNormal;
    
    return float4(color.rgb * attention, 1.0f);
}

float4 CalcPoint(float3 position, Material mat, Light light)
{
    float3 toLight = light.position - position;
    float distanceToLight = length(toLight);
    toLight /= distanceToLight;
    
    float NDotL = dot(toLight, mat.normal);
    float3 finalColor = light.color.rgb * saturate(NDotL);
    
    //float3 camPos = viewInv._41_42_43;
    //float3 toEye = normalize(camPos - position);
    //float3 halfWay = normalize(toEye - toLight);
    //float NDotH = saturate(dot(halfWay, mat.normal));
    //finalColor += light.color.rgb * pow(NDotH, mat.specPow) * mat.specIntensity;
    
    float distanceToLightNormal = 1.0f - saturate(distanceToLight / light.range);
    float attention = distanceToLightNormal * distanceToLightNormal;
    
    return float4(finalColor * mat.diffuseColor.rgb * attention, 1.0f);
}

float4 CalcSpot(float3 tangent, float3 binormal, float3 normal, float4 color,
    float3 worldPos, float3 camPos, Light light, float2 uv)
{
    float3 toLight = light.position - worldPos;
    float distanceToLight = length(toLight);
    toLight /= distanceToLight;
    
    float diffuseIntensity = NormalMapping(tangent, binormal, normal, toLight, uv);
    color *= light.color * saturate(diffuseIntensity) * mDiffuse;
    
    if (diffuseIntensity > 0.0f)
    {
        color += light.color * SpecularMapping(normal, camPos, toLight, worldPos, uv);
    }
    
    float3 dir = -normalize(light.direction);
    float cosAngle = dot(dir, toLight);
    
    float outer = cos(radians(light.outer));
    float inner = 1.0f / cos(radians(light.inner));
    
    float conAttention = saturate((cosAngle - outer) * inner);
    conAttention *= conAttention;
    
    float distanceToLightNormal = 1.0f - saturate(distanceToLight / light.range);
    float attention = distanceToLightNormal * distanceToLightNormal;
    
    return color * attention * conAttention;
}

float4 CalcCapsule(float3 tangent, float3 binormal, float3 normal, float4 color,
    float3 worldPos, float3 camPos, Light light, float2 uv)
{
    float3 direction = normalize(light.direction);
    float3 start = worldPos - light.position;
    float distanceOnLine = dot(start, direction) / light.length;
    distanceOnLine = saturate(distanceOnLine) * light.length;
    
    float3 pointOnLine = light.position + direction * distanceOnLine;
    float3 toLight = pointOnLine - worldPos;
    float distanceToLight = length(toLight);
    toLight /= distanceToLight;
    
    float diffuseIntensity = NormalMapping(tangent, binormal, normal, toLight, uv);
    color *= light.color * saturate(diffuseIntensity) * mDiffuse;
    
    if (diffuseIntensity > 0.0f)
    {
        color += light.color * SpecularMapping(normal, camPos, toLight, worldPos, uv);
    }
    
    float distanceToLightNormal = 1.0f - saturate(distanceToLight / light.range);
    float attention = distanceToLightNormal * distanceToLightNormal;
    
    return color * attention;
}

float ConvertDepthToLinear(float depth)
{
    float linearDepth = perspective._43 / (depth - perspective._33);
    
    return linearDepth;
}

SurfaceData UnpackGBuffer(int2 location)
{
    SurfaceData output;
    
    int3 location3 = int3(location, 0);
    
    float depth = depthTexture.Load(location3).x;
    output.linearDepth = ConvertDepthToLinear(depth);
    
    float4 baseColorSpecInt = colorSpecIntTexture.Load(location3);
    
    output.color = baseColorSpecInt.xyz;
    output.specInt = baseColorSpecInt.w;
    
    output.normal = normalTexture.Load(location3).xyz;
    output.normal = normalize(output.normal * 2.0f - 1.0f);
    
    float specPowerNormal = specPowTexture.Load(location3).x;
    output.specPow = specPowerRange.x + specPowerNormal * specPowerRange.y;
    
    return output;
}

float3 CalcWorldPos(float2 csPos, float linearDepth)
{
    float4 position;
    
    float2 temp;
    temp.x = 1 / perspective._11;
    temp.y = 1 / perspective._22;
    position.xy = csPos.xy * temp * linearDepth;
    position.z = linearDepth;
    position.w = 1.0f;
    
    return mul(position, viewInv).xyz;
}