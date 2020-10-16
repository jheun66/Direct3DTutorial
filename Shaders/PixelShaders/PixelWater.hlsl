#include "PixelHeader.hlsli"

Texture2D reflectionMap : register(t10);
Texture2D refractionMap : register(t11);

cbuffer Water : register(b10)
{
    float4 waterColor;
    
    float waveTranslation;
    float waveScale;
    float waterShininess;
    float waterAlpha;
    
    float waveSpeed;
}

struct PixelInput
{
    float4 pos : SV_Position;
    float2 uv : UV;

    float4 worldPos : Postion0;
    float4 viewDir : ViewDir;
    float4 reflectionPos : Position1;
    float4 refractionPos : Position2;
};


float4 PS(PixelInput input) : SV_Target
{
    // 전체적인 uv 세팅
    input.uv += waveTranslation;
    
    float3 normal = normalMap.Sample(samp, input.uv).rgb * 2.0f - 1.0f;
    
    float2 reflection;
    reflection.x = input.reflectionPos.x / input.reflectionPos.w * 0.5f + 0.5f;
    reflection.y = -input.reflectionPos.y / input.reflectionPos.w * 0.5f + 0.5f;
    reflection += normal.xy * waveScale;
    float4 reflectionColor = reflectionMap.Sample(samp, reflection);
    
    float2 refraction;
    refraction.x = input.refractionPos.x / input.refractionPos.w * 0.5f + 0.5f;
    refraction.y = -input.refractionPos.y / input.refractionPos.w * 0.5f + 0.5f;
    refraction += normal.xy * waveScale;
    float4 refractionColor = refractionMap.Sample(samp, refraction);
    
    float3 heightView = input.viewDir.yyy;
    float r = (1.2f - 1.0f) / (1.2f / 1.0f);
    float fresnel = saturate(min(1, r + (1 - r) * pow(1 - dot(normal, heightView), 2)));
    float4 diffuse = lerp(reflectionColor, refractionColor, fresnel) * waterColor;
 
    return float4(diffuse.rgb, waterAlpha);
}