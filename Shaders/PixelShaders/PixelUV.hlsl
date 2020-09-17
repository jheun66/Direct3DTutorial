#include "PixelHeader.hlsli"

struct PixelInput
{
    float4 pos : SV_Position;
    float2 uv : UV;
};


float4 PS(PixelInput input) : SV_Target
{
    return diffuseMap.Sample(samp, input.uv) * mDiffuse;
}