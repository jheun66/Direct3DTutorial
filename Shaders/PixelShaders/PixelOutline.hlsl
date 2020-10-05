#include "PixelHeader.hlsli"

cbuffer Info : register(b10)
{
    int value;
    int range;
}

cbuffer ImageSize : register(b11)
{
    float2 imageSize;
}

struct PixelInput
{
    float4 pos : SV_Position;
    float2 uv : UV;
};


float4 PS(PixelInput input) : SV_Target
{
    float count = 0;
    
    for (int y = -1; y <= 1;y++)
    {
        for (int x = -1; x <= 1; x++)
        {
            float2 offset = (float2(x, y) / imageSize) * range;
            float4 result = diffuseMap.Sample(samp, input.uv + offset);
            
            count += result.a;
        }
    }
    
    if(count > value && count < 9 - value)
        return mDiffuse;
    
    return float4(0, 0, 0, 0);
}