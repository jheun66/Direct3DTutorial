#include "PixelHeader.hlsli"

static const float g = -0.990f;
static const float g2 = -0.990f * -0.990f;
static const float Exposure = -2.0f;

cbuffer Scatter : register(b10)
{
    float starIntensity;
}

Texture2D rayleighMap : register(t10);
Texture2D mieMap : register(t11);

float GetRayleighPhase(float c)
{
    return 0.75f * (1.0f + c);
}

float GetMiePhase(float c, float c2)
{
    float3 result = 0;
    result.x = 1.5f * ((1.0f - g2) / (2.0f + g2));
    result.y = 1.0f + g2;
    result.z = 2.0f + g;
    return result.x * (1.0f + c2) / pow(result.y - result.z * c, 1.5f);
}

// 모니터 색을 현실적으로 보간시켜줌
float3 HDR(float3 LDR)
{
    return 1.0f - exp(Exposure * LDR);
}

struct PixelInput
{
    float4 pos : SV_Position;
    float2 uv : UV;
    float3 oPosition : Position;
};


float4 PS(PixelInput input) : SV_Target
{
    float3 sunDirection = -normalize(lights[0].direction);
    
    float temp = dot(sunDirection, input.oPosition) / length(input.oPosition);
    float temp2 = temp * temp;
    
    float3 rSamples = rayleighMap.Sample(samp, input.uv);
    float3 mSamples = mieMap.Sample(samp, input.uv);
    
    float3 color = 0;
    color = GetRayleighPhase(temp2) * rSamples + GetMiePhase(temp, temp2) * mSamples;
    
    color = HDR(color);
    
    color += max(0, (1 - color.rgb)) * float3(0.05f, 0.05f, 0.1f);
    
    float intensity = saturate(starIntensity);
    
    return float4(color, 1) + diffuseMap.Sample(samp, input.uv) * intensity;
}