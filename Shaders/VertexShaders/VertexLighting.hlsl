#include "VertexHeader.hlsli"

struct VertexInput
{
	//          ½Ã¸àÆ½ ³×ÀÓ
    float4 pos : Position;
    float2 uv : UV;
    float3 normal : NORMAL;
    // ¹ý¼± º¤ÅÍ
    float3 tangent : TANGENT;
};

struct PixelInput
{
    float4 pos : SV_Position;
    float2 uv : UV;
    float3 normal : NORMAL;
    float4 worldPos : Position0;
    float3 cameraPos : Position1;
};


PixelInput VS(VertexInput input)
{
    PixelInput output;
    
    matrix boneWorld = mul(bones[index], world);
    
    output.pos = mul(input.pos, boneWorld);
    output.worldPos = output.pos;
    
    float3 camPos = invView._41_42_43;
    output.cameraPos = camPos;
    
    output.pos = mul(output.pos, view);
    output.pos = mul(output.pos, projection);

    output.uv = input.uv;

    output.normal = mul(input.normal, (float3x3) boneWorld);
    
    return output;
}