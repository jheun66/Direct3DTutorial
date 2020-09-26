#include "PixelHeader.hlsli"

struct PixelInput
{
    float4 pos : SV_Position;
    float2 uv : UV;
    float3 normal : NORMAL;
    float3 worldPos : Position0;
    float3 camPos : Position1;
};

float4 PS(PixelInput input) : SV_Target
{
    float4 albedo = float4(1, 1, 1, 1);
    if (hasMap[0])
        albedo = diffuseMap.Sample(samp, input.uv);
    
    float3 normal = normalize(input.normal);
    
    float4 result = CalcAmbient(normal, albedo);
    
    
    for (uint i = 0; i < lightCount;i++)
    {
        if (!lights[i].active)
            continue;
        
        [flatten]
        if(lights[i].type == 0)
        {
            result += CalcDirection(normal, albedo, input.worldPos, input.camPos, lights[i]);
        }
        else if (lights[i].type == 1)
        {
            result += CalcPoint(normal, albedo, input.worldPos, input.camPos, lights[i]);
        }
        else if (lights[i].type == 2)
        {
            result += CalcSpot(normal, albedo, input.worldPos, input.camPos, lights[i]);
        }
        else if (lights[i].type == 3)
        {
            result += CalcCapsule(normal, albedo, input.worldPos, input.camPos, lights[i]);
        }
    }
    
    return result;
}