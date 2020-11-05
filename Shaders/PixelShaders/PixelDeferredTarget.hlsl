#include "PixelHeader.hlsli"

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

struct PixelOutput
{
    float4 diffuse : SV_Target0;
    float4 specular : SV_Target1;
    float4 emissive : SV_Target2;
    float4 normal : SV_Target3;
    float4 tangent : SV_Target4;
};

PixelOutput PS(PixelInput input)
{
    PixelOutput output;
    output.diffuse = diffuseMap.Sample(samp, input.uv);
    output.specular = specularMap.Sample(samp, input.uv);
    output.emissive = mEmissive;
    
    output.normal = float4(input.normal, 1);
    output.tangent = float4(input.tangent, 1);

    return output;
}