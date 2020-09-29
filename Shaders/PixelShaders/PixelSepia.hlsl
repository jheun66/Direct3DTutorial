#include "PixelHeader.hlsli"

struct PixelInput
{
    float4 pos : SV_Position;
    float2 uv : UV;
};


float4 PS(PixelInput input) : SV_Target
{
    float4 color = diffuseMap.Sample(samp, input.uv);
    
    float4 sepia;
    sepia.a = color.a;
    
    sepia.r = dot(color.rgb, float3(0.393f, 0.769f, 0.189f));
    sepia.g = dot(color.rgb, float3(0.349f, 0.686f, 0.168f));
    sepia.b = dot(color.rgb, float3(0.272f, 0.534f, 0.131f));
    
    return sepia * mDiffuse;
}