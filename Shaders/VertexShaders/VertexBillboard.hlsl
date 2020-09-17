#include "VertexHeader.hlsli"

struct VertexOutput
{
    //          Rasterizer�� �ƴ϶� Geometry�� ������ SV ����
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