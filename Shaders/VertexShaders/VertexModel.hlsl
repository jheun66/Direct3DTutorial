#include "VertexHeader.hlsli"

struct PixelInput
{
    float4 pos : SV_Position;
    float2 uv : UV;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 binormal : BINORMAL;
    float3 worldPos : POSITION0;
    float3 camPos : POSITION1;
};


PixelInput VS(VertexUVNormalTangentBlend input)
{
    PixelInput output;
    
    matrix boneWorld = BoneWorld(input.indices, input.weights);
    
    output.pos = mul(input.pos, boneWorld);
    
    output.worldPos = output.pos;
    output.camPos = invView._41_42_43;
    
    output.pos = mul(output.pos, view);
    output.pos = mul(output.pos, projection);

    output.uv = input.uv;

    output.normal = mul(input.normal, (float3x3) boneWorld);
    output.tangent = mul(input.tangent, (float3x3) boneWorld);
    output.binormal = cross(output.normal, output.tangent);
    
    return output;
}