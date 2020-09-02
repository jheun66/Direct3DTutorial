#include "VertexHeader.hlsli"

struct VertexInput
{
	//          �ø�ƽ ����
    float4 pos : Position;
    float2 uv : UV;
    float3 normal : NORMAL;
    // ���� ����
    float3 tangent : TANGENT;
};

struct PixelInput
{
    float4 pos : SV_Position;
    float2 uv : UV;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 binormal : BINORMAL;
    float3 viewDir : VIEWDIR;
};


PixelInput VS(VertexInput input)
{
    PixelInput output;
    
    matrix boneWorld = mul(bones[index], world);
    
    output.pos = mul(input.pos, boneWorld);
    
    float3 camPos = invView._41_42_43;
    output.viewDir = normalize(output.pos.xyz - camPos);
    
    output.pos = mul(output.pos, view);
    output.pos = mul(output.pos, projection);

    output.uv = input.uv;

    output.normal = mul(input.normal, (float3x3) boneWorld);
    output.tangent = mul(input.tangent, (float3x3) boneWorld);
    output.binormal = cross(output.normal, output.tangent);
    
    return output;
}