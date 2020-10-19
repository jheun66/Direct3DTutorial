#include "VertexHeader.hlsli"

cbuffer Reflection : register(b10)
{
    matrix reflection;
}

struct PixelInput
{
    float4 pos : SV_Position;
    float2 uv : UV;
    
    float4 worldPos      : Postion0;
    float3 viewDir : ViewDir;
    float4 reflectionPos : Position1;
    float4 refractionPos : Position2;
};


PixelInput VS(VertexUV input)
{
    PixelInput output;
	
    output.pos = mul(input.pos, world);
    output.worldPos = output.pos;
    
    output.viewDir = normalize(output.pos.xyz - invView._41_42_43);
    
    output.reflectionPos = mul(output.pos, reflection);
    output.reflectionPos = mul(output.reflectionPos, projection);
    
    output.pos = mul(output.pos, view);
    output.pos = mul(output.pos, projection);
    
    output.refractionPos = output.pos;
    
    output.uv = input.uv;
    
    return output;
}