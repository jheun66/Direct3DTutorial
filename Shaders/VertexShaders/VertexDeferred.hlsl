#include "VertexHeader.hlsli"

struct PixelInput
{
    float4 pos : SV_Position;
    float2 uv : UV;
};

static const float2 arrBasePos[4] =
{
    float2(-1.0f, 1.0f),
    float2(1.0f, 1.0f),
    float2(-1.0f, -1.0f),
    float2(1.0f, -1.0f)
};

PixelInput VS(uint vertexID : SV_VertexID)
{
    PixelInput output;
	
    output.pos = float4(arrBasePos[vertexID].xy, 0.0f, 1.0f);
    output.uv = output.pos.xy;
    
    return output;
}