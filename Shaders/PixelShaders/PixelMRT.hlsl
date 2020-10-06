#include "PixelHeader.hlsli"

cbuffer ImageSize : register(b10)
{
    float2 imageSize;
}

struct PixelInput
{
    float4 pos : SV_Position;
    float2 uv : UV;
};

struct PixelOutput
{
    float4 color1 : SV_Target0;
    float4 color2 : SV_Target1;
};

// 소벨 연산자(Sobel Operator) : 외각선쪽에서 사용
// 수직 마스크
static const float3x3 Kx =
{
    -1, 0, 1,
    -2, 0, 2,
    -1, 0, 1
};

// 수평 마스크
static const float3x3 Ky =
{
    1, 2, 1,
    0, 0, 0,
    -1, -2, -1
};

float4 EdgeLine(float2 uv)
{
    float lX = 0;
    float lY = 0;
    
    for (int y = -1; y <= 1; y++)
    {
        for (int x = -1; x <= 1; x++)
        {
            float2 offset = float2(x, y) / imageSize;
            float4 color = diffuseMap.Sample(samp, uv + offset);
            float luminance = dot(color.rgb, float3(0.3f, 0.59f, 0.11f));
            
            lX += luminance * Kx[y + 1][x + 1];
            lY += luminance * Ky[y + 1][x + 1];
        }
    }
    
    float L = sqrt(lX * lX + lY * lY);
    
    return float4(L.xxx, 1.0f);
}

PixelOutput PS(PixelInput input)
{
    PixelOutput output;
    
    float4 color = diffuseMap.Sample(samp, input.uv);
    
    output.color1 = float4(1.0f - color.rgb, 1.0f);
    
    output.color2 = EdgeLine(input.uv);
    
    return output;
    //return diffuseMap.Sample(samp, input.uv) * mDiffuse;
}