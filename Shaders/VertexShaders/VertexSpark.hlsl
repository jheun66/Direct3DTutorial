#include "VertexHeader.hlsli"

cbuffer Spark : register(b10)
{
    float duration;
    float time;
    float gravity;
}

struct VertexInput
{
    float4 pos : Position;
    float2 size : Size;
    float3 velocity : Velocity;
};

struct VertexOutput
{
    //          Rasterizer�� �ƴ϶� Geometry�� ������ SV ����
    float3 pos : Position;
    float2 size : Size;
    float time : Time;
};


VertexOutput VS(VertexInput input)
{
    VertexOutput output;
	
    output.time = time / duration;
    
    input.pos = mul(input.pos, world);
    output.pos = input.pos.xyz + input.velocity * time;
    //output.pos = mul(output.pos, (float3x3) world);
   
    output.size = input.size;
    
    return output;
}