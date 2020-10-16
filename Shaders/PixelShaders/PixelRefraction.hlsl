#include "PixelHeader.hlsli"

Texture2D refractionMap : register(t11);

cbuffer Time : register(b10)
{
    float time;
}

struct PixelInput
{
    float4 pos : SV_Position;
    float2 uv : UV;
    float3 normal : Normal;
    float3 viewDir : ViewDir;
    float4 wvpPos : Position;
};


float4 PS(PixelInput input) : SV_Target
{
    float4 refPos = input.wvpPos;
    
    float2 refraction;
    refraction.x = refPos.x / refPos.w * 0.5f + 0.5f;
    refraction.y = -refPos.y / refPos.w * 0.5f + 0.5f;
    
    //refraction += refract(input.viewDir, input.normal, 0.15f).xy;
    
    input.uv.x += (time * 0.25f);
    input.uv.y += (time * 0.5f);
    
    float4 normal = normalMap.Sample(samp, input.uv) * 2.0f - 1.0f;
    refraction += normal.xy * 0.05f;
  
    return refractionMap.Sample(samp, refraction);
}