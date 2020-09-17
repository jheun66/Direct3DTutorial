#include "VertexHeader.hlsli"

struct VertexOutput
{
    //          Rasterizer가 아니라 Geometry에 보내서 SV 제거
    float3 pos : Position;
    float2 size : Size;
};


VertexOutput VS(VertexSize input)
{
    VertexOutput output;
	
    output.pos = input.pos.xyz;
    output.size = input.size;
    
    return output;
}