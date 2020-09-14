#include "VertexHeader.hlsli"

struct PixelInput
{
    float4 pos : SV_Position;
    float3 oPosition : POSITION;
};


PixelInput VS(VertexUV input)
{
    PixelInput output;
	
    output.pos.xyz = mul(input.pos.xyz, (float3x3) view);
    output.pos.w = 1.0f;
    
    output.pos = mul(output.pos, projection);
    
    output.oPosition = input.pos.xyz;
	
    return output;
}