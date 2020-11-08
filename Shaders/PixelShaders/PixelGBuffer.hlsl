#include "PixelHeader.hlsli"

struct PixelInput
{
    float4 pos : SV_Position;
    float2 uv : UV;
    float3 normal : Normal;
    float3 tangent : Tangent;
    float3 binormal : Binormal;
    float3 worldPos : Position0;
    float3 camPos : Position1;
};

struct PixelOutput
{
    float4 colorSpecInt : SV_Target0;
    float4 normal : SV_Target1;
    float4 specPow : SV_Target2;
};

PixelOutput PackGBuffer(float3 baseColor, float3 normal, float specIntensity, float specPower)
{
    PixelOutput output;
    
    float specPowerNorm = (specPower - specPowerRange.x) / specPowerRange.y;
    
    output.colorSpecInt = float4(baseColor.rgb, specIntensity);
    output.normal = float4(normal * 0.5f + 0.5f, 0.0f);
    output.specPow = float4(specPowerNorm, 0, 0, 0);
    
    return output;
}
PixelOutput PS(PixelInput input)
{
    float3 diffuseColor = diffuseMap.Sample(samp, input.uv).rgb;
    diffuseColor *= diffuseColor;
    
    float specIntensity = 1.0f;
    
    if (hasMap[1])
        specIntensity = specularMap.Sample(samp, input.uv).r;
    
    float3 normal = GetMappingNormal(input.tangent,
    input.binormal, input.normal, input.uv);
    
    return PackGBuffer(diffuseColor, normal,
    specIntensity, mSpecular.a);
}