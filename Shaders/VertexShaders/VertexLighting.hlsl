#include "VertexHeader.hlsli"

struct PixelInput
{
    float4 pos : SV_Position;
    float2 uv : UV;
    float3 normal : NORMAL;
    float3 worldPos : Position0;
    float3 camPos : Position1;
};


PixelInput VS(VertexUVNormalTangentBlend input)
{
    PixelInput output;
    
    matrix boneWorld = BoneWorld(input.indices, input.weights);
    
    output.pos = mul(input.pos, boneWorld);
    
    output.worldPos = output.pos.xyz;
    output.camPos = invView._41_42_43;
    
    output.pos = mul(output.pos, view);
    output.pos = mul(output.pos, projection);

    output.uv = input.uv;

    output.normal = mul(input.normal, (float3x3) boneWorld);
    
    return output;
}