#include "PixelHeader.hlsli"

struct PixelInput
{
    float4 pos : SV_Position;
    float2 uv : UV;
};

float4 PS(PixelInput input) : SV_Target
{
    SurfaceData gbd = UnpackGBuffer(input.pos.xy);
    
    Material mat;
    mat.normal = gbd.normal;
    mat.diffuseColor.xyz = gbd.color;
    mat.diffuseColor.w = 1.0f;
    mat.specPow = specPowerRange.x + specPowerRange.y * gbd.specPow;
    mat.specIntensity = gbd.specInt;
    
    float3 position = CalcWorldPos(input.uv, gbd.linearDepth);
    
    float4 finalColor = 0;
    finalColor.xyz = CalcAmbient(mat.normal, mat.diffuseColor);
        
    for (int i = 0; i < lightCount; i++)
    {
        if (!lights[i].active)
            continue;
        
        [flatten]
        if (lights[i].type == 0)
        {
            finalColor += CalcDirection(position, mat, lights[i]);
        }
        else if (lights[i].type == 1)
        {
            finalColor += CalcPoint(position, mat, lights[i]);
        }
    }
    
    return finalColor;
}