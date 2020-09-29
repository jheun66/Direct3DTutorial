#include "PixelHeader.hlsli"

cbuffer Info : register(b10)
{
    int selected;
    int value;
    int value1;
    int value2;
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

float4 Mosaic(float2 uv)
{
    //int x = (int) (uv.x * value);
    //int y = (int) (uv.y * value);
    
    float2 temp = floor(uv * value) / value;
    
    return diffuseMap.Sample(samp, temp);
}

static const float2 edges[8] =
{
    float2(-1, -1), float2(0, -1), float2(1, -1),
    float2(-1, -0),                 float2(1, 0),
    float2(-1, 1), float2(0, 1), float2(1, 1),
};

float4 Blur(float2 uv)
{
    float4 result = 0;
 
    for (int i = 0; i < value;i++)
    {
        float2 pixelSize = (i + 1) / imageSize;
        
        for (int j = 0; j < 8; j++)
        {
            float2 temp = edges[j] * pixelSize + uv;
        
            result += diffuseMap.Sample(samp, temp);
        }

    }
    
    result /= value * 8;
    
    return result;
}

// 정규 분포값
static const float weights[13] =
{
    0.0561f, 0.1353f, 0.2730f, 0.4868f, 0.7261f, 0.9231f,
    1.0f,
    0.9231f, 0.7261f, 0.4868f, 0.2730f, 0.1353f, 0.0561f
};

float4 GaussianBlur(float2 uv)
{
    float2 pixelSize = 1.0f / imageSize;
    
    float sum = 0;
    float4 result = 0;
    
    for (int i = -6; i <= 6; i++)
    {
        float2 temp = uv + float2(pixelSize.x * i * value, 0);
        result += weights[6 + i] * diffuseMap.Sample(samp, temp);
        
        temp = uv + float2(0, pixelSize.y * i * value);
        result += weights[6 + i] * diffuseMap.Sample(samp, temp);
        
        sum += weights[6 + i] * 2;
    }
    
    result /= sum;
    
    return result;
}

float4 RadialBlur(float2 uv)
{
    float2 radiusUV = uv - float2(0.5f, 0.5f);
    float r = length(radiusUV);
    
    radiusUV /= r;
    r = saturate(2 * r / value1);
    
    float2 delta = -radiusUV * r * r * value2 / value;
    
    float4 result = 0;
    for (int i = 0; i < value;i++)
    {
        result += diffuseMap.Sample(samp, uv);
        uv += delta;
    }
    
    result /= value;
    
    return result;
}

float4 PS(PixelInput input) : SV_Target
{
    [branch]
    if(selected == 1)
        return Mosaic(input.uv);
    else if (selected == 2)
        return Blur(input.uv);
    else if (selected == 3)
        return GaussianBlur(input.uv);
    else if (selected == 4)
        return RadialBlur(input.uv);
    return diffuseMap.Sample(samp, input.uv) * mDiffuse;
}