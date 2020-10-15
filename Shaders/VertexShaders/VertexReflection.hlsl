#include "VertexHeader.hlsli"

cbuffer Reflection : register(b10)
{
    matrix reflection;
}

struct VertexInput
{
    float4 pos : Position;
    float2 uv : UV;
    float3 normal : Normal;
    float3 tangent : Tangent;
    float4 indices : BlendIndices;
    float4 weights : BlendWeights;
    
    matrix transform : Instance;
    uint instanceID : SV_InstanceID;
};

struct PixelInput
{
    float4 pos : SV_Position;
    
    float4 wvpPositionSub : Position;
};


PixelInput VS(VertexInput input)
{
    PixelInput output;
    
    matrix transform = 0;
    
    [flatten]
    if (modelType)
    {
        transform = SkinWorld(input.instanceID, input.indices, input.weights);
        transform = mul(transform, input.transform);
    }
    else
        transform = input.transform;
	
    output.pos = mul(input.pos, transform);
    
    output.wvpPositionSub = mul(output.pos, reflection);
    output.wvpPositionSub = mul(output.wvpPositionSub, projection);
    
    output.pos = mul(output.pos, view);
    output.pos = mul(output.pos, projection);
    
    return output;
}