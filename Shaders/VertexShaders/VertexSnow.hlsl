#include "VertexHeader.hlsli"

cbuffer Snow : register(b10)
{
    float3 velocity;
    float drawDistance;
    
    float4 color;
    float3 origin;
    float time;
    
    float3 size;
    float turbulence;
}

struct VertexInput
{
    float4 pos : Position;
    float scale : Scale;
    float2 random : Random;
};

struct VertexOutput
{
    //          Rasterizer가 아니라 Geometry에 보내서 SV 제거
    float3 pos : Position;
    float2 size : Size;
    float distance : Distance;
    float4 color : Color;
    float3 velocity : Velocity;
};


VertexOutput VS(VertexInput input)
{
    VertexOutput output;
	    
    float3 displace = time * velocity;

    input.pos.y = origin.y + size.y - (input.pos.y - displace.y) % size.y;
    
    input.pos.x += cos(time - input.random.x) * turbulence;
    input.pos.z += cos(time - input.random.y) * turbulence;

    input.pos.xyz = origin + (size + (input.pos.xyz + displace) % size) % size - (size * 0.5f);
    
    output.velocity = velocity;
    output.color = color;
    
    output.pos = input.pos;
    output.size = float2(input.scale, input.scale);
    
    output.distance = drawDistance;
    
    return output;
}