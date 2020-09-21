#include "VertexHeader.hlsli"

cbuffer Rain : register(b10)
{
    float3 velecity;
    float drawDistance;
    
    float4 color;
    float3 origin;
    float time;
    
    float3 size;
}

struct VertexOutput
{
    //          Rasterizer가 아니라 Geometry에 보내서 SV 제거
    float3 pos : Position;
    float2 size : Size;
    float2 distance : Distance;
    float4 color : Color;
    float3 velocity : Velocity;
};


VertexOutput VS(VertexSize input)
{
    VertexOutput output;
	
    float3 v = velecity;
    v.xz /= input.size.y * 0.1f;
    output.velocity = v;
    
    float3 displace = time * v;
    input.pos.xyz = origin.xyz + 
    (size.xyz + (input.pos.xyz + displace.xyz) % size.xyz) % size.xyz - (size.xyz * 0.5f);

    float alpha = cos(time + (input.pos.x + input.pos.z));
    alpha = saturate(1.5f + alpha / drawDistance * 2);
    
    output.distance.x = drawDistance;
    output.distance.y = alpha;
    
    output.color = color;
    
    output.pos = input.pos;
    output.size = input.size;
    
    return output;
}