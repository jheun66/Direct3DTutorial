#include "PixelHeader.hlsli"

cbuffer Point : register(b10)
{
    float3 pointLightPosition;
    float pointLightRange;
    float4 pointLightColor;
}

struct PixelInput
{
    float4 pos : SV_Position;
    float2 uv : UV;
    float3 normal : NORMAL;
    float4 worldPos : Position0;
    float3 cameraPos : Position1;
};

float4 CalcPoint(float3 normal, float4 color, float3 worldPos, float3 camPos)
{
    float3 toLight = pointLightPosition - worldPos;
    float3 distanceToLight = length(toLight);
    toLight /= distanceToLight;
    
    float diffuseIntensity = dot(toLight, normal);
    color *= pointLightColor * saturate(diffuseIntensity) * mDiffuse;
    
    if(diffuseIntensity > 0.0f)
    {
        float3 toEye = normalize(camPos - worldPos);
        float3 halfWay = normalize(toEye + toLight);
        float specularIntensity = saturate(dot(halfWay, normal));
        color += pointLightColor * pow(specularIntensity, specExp) * mSpecular;

    }
    
    float distanceToLightNormal = 1.0f - saturate(distanceToLight / pointLightRange);
    float attention = distanceToLightNormal * distanceToLightNormal;
 
    return color * attention;
}

float4 PS(PixelInput input) : SV_Target
{
    float4 albedo = float4(1, 1, 1, 1);
    if (hasMap[0])
        albedo = diffuseMap.Sample(samp, input.uv);
    
    float3 normal = normalize(input.normal);
    
    float4 result = CalcAmbient(normal, albedo);
    
    float4 pointColor = CalcPoint(normal, albedo, input.worldPos.xyz, input.cameraPos);
    
    return result + pointColor;
}