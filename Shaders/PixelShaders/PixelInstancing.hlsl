#include "PixelHeader.hlsli"

Texture2D map[3] : register(t10);

struct PixelInput
{
    float4 pos : SV_Position;
    float2 uv : UV;
    uint index : Index;
};


float4 PS(PixelInput input) : SV_Target
{
    float4 color = 0;
    
    [flatten]
    if (input.index == 0)
        color = map[0].Sample(samp, input.uv);
    else if (input.index == 1)
        color = map[1].Sample(samp, input.uv);
    else
        color = map[2].Sample(samp, input.uv);
    
    return color;
}