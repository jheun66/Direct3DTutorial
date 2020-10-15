#include "PixelHeader.hlsli"

Texture2D reflectionMap : register(t10);

struct PixelInput
{
    float4 pos : SV_Position;
    
    float4 wvpPositionSub : Position;
};


float4 PS(PixelInput input) : SV_Target
{
    float4 refPos = input.wvpPositionSub;
    
    float2 reflection;
    reflection.x = refPos.x / refPos.w * 0.5f + 0.5f;
    reflection.y = -refPos.y / refPos.w * 0.5f + 0.5f;
    
    
    return reflectionMap.Sample(samp, reflection);
}