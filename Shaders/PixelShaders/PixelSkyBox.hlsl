#include "PixelHeader.hlsli"

TextureCube cubeMap : register(t10);

struct PixelInput
{
    float4 position : SV_Position;
    float3 oPosition : Position;
};

float4 PS(PixelInput input) : SV_Target
{
    return cubeMap.Sample(samp, input.oPosition);
}