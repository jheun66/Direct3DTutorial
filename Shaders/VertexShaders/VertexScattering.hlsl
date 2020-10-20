#include "VertexHeader.hlsli"

struct PixelInput
{
    float4 pos : SV_Position;
    float2 uv : UV;
    float3 oPosition : Position;
};


PixelInput VS(VertexUV input)
{
    PixelInput output;
	
    output.pos = mul(input.pos, world);
    output.pos = mul(output.pos, view);
    output.pos = mul(output.pos, projection);
	
    output.oPosition = -input.pos.xyz;
    
    output.uv = input.uv;
    
    return output;
}