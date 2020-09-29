#include "PixelHeader.hlsli"

struct PixelInput
{
    float4 pos : SV_Position;
    float2 uv : UV;
};


float4 PS(PixelInput input) : SV_Target
{
    float4 color = diffuseMap.Sample(samp, input.uv);
    
    //float scale = (color.r + color.g + color.b) / 3;
    //float3 grayColor;
    //grayColor.r = color.r * 0.3f;
    //grayColor.g = color.g * 0.59f;
    //grayColor.b = color.b * 0.11f;
    
    //float scale = grayColor.r + grayColor.g + grayColor.b;
    
    // 내적이용하는게 더 빠름
    float scale = dot(color.rgb, float3(0.3f, 0.59f, 0.11f));
    
    return float4(scale.xxx, color.a) * mDiffuse;
}