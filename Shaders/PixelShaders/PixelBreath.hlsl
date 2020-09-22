#include "PixelHeader.hlsli"

struct PixelInput
{
    float4 pos : SV_Position;
    float2 uv : UV;
    float time : Time;
    float3 color : Color;
};


float4 PS(PixelInput input) : SV_Target
{
    float4 result = diffuseMap.Sample(samp, input.uv) * mDiffuse;
     
    float alpha = saturate(1 - input.time);
    if(input.time == 0)
        alpha = 0.0f;
    
    return float4(result.rgb * input.color, alpha);
}