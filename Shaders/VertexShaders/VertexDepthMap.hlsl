#include "VertexHeader.hlsli"

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
    if (modelType == 0)
        boneWorld = BoneWorld(input.indices, input.weights);
    else if (modelType == 1)
    {
        boneWorld = SkinWorld(0, input.indices, input.weights);
        boneWorld = mul(boneWorld, world);
    }
    
    output.pos = mul(input.pos, boneWorld);
    output.pos = mul(output.pos, view);
    output.pos = mul(output.pos, projection);
    
    return output;
}