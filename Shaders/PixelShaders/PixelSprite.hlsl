#include "PixelHeader.hlsli"

cbuffer Sprite : register(b10)
{
    float2 maxFrame;
    float2 curFrame;
};

struct PixelInput
{
    float4 pos : SV_Position;
    float2 uv : UV;
};


float4 PS(PixelInput input) : SV_Target
{
    float2 uv = input.uv / maxFrame + curFrame / maxFrame;
    return diffuseMap.Sample(samp, uv) * mDiffuse;
}