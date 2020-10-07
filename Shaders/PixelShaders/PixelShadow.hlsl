#include "PixelHeader.hlsli"

cbuffer Qualitiy : register(b10)
{
    int quality;
}

cbuffer Size : register(b11)
{
    float2 mapSize;
}

Texture2D depthMap : register(t10);

struct PixelInput
{
    float4 pos : SV_Position;
    float2 uv : UV;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 binormal : BINORMAL;
    float3 worldPos : POSITION0;
    float3 camPos : POSITION1;
    float4 clipPos : POSITION2;
};
//                          반환값의 시멘틱 네임(SV : system value)
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
    
    // w로 나누는건 정규화
    float currentDepth = input.clipPos.z / input.clipPos.w;
    
    float2 uv = input.clipPos.xy / input.clipPos.w;
    // uv 범위로 바꿔줌
    uv.y = -uv.y;
    uv = uv * 0.5f + 0.5f;
    
    if(uv.x < 0.0f || uv.x > 1.0f || uv.y < 0.0f || uv.y > 1.0f)
        return result;
    
    if(currentDepth < 0.0f || currentDepth > 1.0f)
        return result;
    
    float shadowDepth = depthMap.Sample(samp, uv).r;
    
    
    float factor = 0;
    
    [flatten]
    if (quality == 0)
    {
        if(currentDepth > shadowDepth + 0.0001f)
            factor = 0.5f;
    }
    else
    {
        float avg = 0;
        float sum = 0;
        
        for (float y = -0.5f; y <= 0.5f; y += 1.0f)
        {
            for (float x = -0.5f; x <= 0.5f; x += 1.0f)
            {
                float2 offset = float2(x / mapSize.x, y / mapSize.y);
                sum += depthMap.Sample(samp, uv + offset).r;
                avg++;
            }
        }
        
        factor = sum / avg;
    }
    
    factor = saturate(factor);
    
    if (factor < 1)
        factor = 1.0f - factor;
    
    return result * factor;
}