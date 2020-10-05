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
//                          ¹ÝÈ¯°ªÀÇ ½Ã¸àÆ½ ³×ÀÓ(SV : system value)
float4 PS(PixelInput input) : SV_Target
{
    float4 albedo = float4(1, 1, 1, 1);
    if (hasMap[0])
        albedo = diffuseMap.Sample(samp, input.uv);
        
    float4 result = CalcAmbient(input.normal, albedo);
    
    
    for (uint i = 0; i < lightCount; i++)
    {
        if (!lights[i].active)
            continue;
        
        [flatten]
        if (lights[i].type == 0)
        {
            result += CalcDirection(input.tangent, input.binormal, input.normal, 
            albedo, input.worldPos, input.camPos, lights[i], input.uv);
        }
        else if (lights[i].type == 1)
        {
            result += CalcPoint(input.tangent, input.binormal, input.normal,
            albedo, input.worldPos, input.camPos, lights[i], input.uv);
        }
        else if (lights[i].type == 2)
        {
            result += CalcSpot(input.tangent, input.binormal, input.normal,
            albedo, input.worldPos, input.camPos, lights[i], input.uv);
        }
        else if (lights[i].type == 3)
        {
            result += CalcCapsule(input.tangent, input.binormal, input.normal,
            albedo, input.worldPos, input.camPos, lights[i], input.uv);
        }
    }
    
    return result;
}