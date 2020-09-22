#include "PixelHeader.hlsli"

struct PixelInput
{
    float4 pos : SV_Position;
    float2 uv : UV;
    float4 color : Color;
    float alpha : Alpha;
};


float4 PS(PixelInput input) : SV_Target
{
    float4 result = diffuseMap.Sample(samp, input.uv) * mDiffuse;
    
    result.rgb *= input.color.rgb * (1 + input.alpha) * 2.0f;
    result.a = result.a * (1.0f - input.alpha) * 1.5f;
    
    return result;
}