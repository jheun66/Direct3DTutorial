#include "PixelHeader.hlsli"

cbuffer StartColor : register(b10)
{
    float4 startColor;
    
}

cbuffer EndColor : register(b11)
{
    float4 endColor;
}

struct PixelInput
{
    float4 pos : SV_Position;
    float2 uv : UV;
    float time : Time;
};


float4 PS(PixelInput input) : SV_Target
{
    float4 result = diffuseMap.Sample(samp, input.uv) * mDiffuse;
    
    float4 color = lerp(startColor, endColor, input.time);
    
    return result * color;
}