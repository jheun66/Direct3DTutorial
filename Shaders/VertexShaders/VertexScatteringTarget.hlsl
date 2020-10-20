#include "VertexHeader.hlsli"

struct PixelInput
{
    float4 position : SV_Position;
    float2 uv : UV;
};

PixelInput VS(VertexUV input)
{
    PixelInput output;
    output.position = input.pos;
    output.uv = input.uv;
    
    return output;
    
}