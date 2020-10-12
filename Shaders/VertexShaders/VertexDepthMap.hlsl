#include "VertexHeader.hlsli"

cbuffer Type : register(b10)
{
    int type;
}

struct VertexInput
{
    float4 pos : Position;
    float2 uv : UV;
    float3 normal : Normal;
    float3 tangent : Tangent;
    float4 indices : BlendIndices;
    float4 weights : BlendWeights;
};

struct PixelInput
{
    float4 pos : SV_Position;
};


PixelInput VS(VertexInput input)
{
    PixelInput output;
    
    matrix boneWorld = world;
    
    [flatten]
    if(type == 1)
        boneWorld = mul(bones[index], world);
    else if(type ==2)
    {
        boneWorld = SkinWorld(0, input.indices, input.weights);
        boneWorld = mul(boneWorld, world);
    }
    
    output.pos = mul(input.pos, boneWorld);
    output.pos = mul(output.pos, view);
    output.pos = mul(output.pos, projection);
    
    return output;
}