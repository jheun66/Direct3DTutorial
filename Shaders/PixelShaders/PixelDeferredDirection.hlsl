#include "PixelHeader.hlsli"

Texture2D depthTexture : register(t10);
Texture2D colorSpecIntTexture : register(t11);
Texture2D normalTexture : register(t12);
Texture2D specPowTexture : register(t13);

struct SurfaceData
{
    float linearDepth;
    float3 color;
    float3 normal;
    float specInt;
    float specPow;
};

float ConVertDepthToLinear(float depth)
{
    float linearDepth = perspective[3][2] / (depth + perspective[2][2]);
    return linearDepth;
}

SurfaceData UnpackGBuffer(int2 location)
{
    SurfaceData output;
    
    int3 location3 = int3(location, 0);
    
    float depth = depthTexture.Load(location3).x;
    output.linearDepth = ConVertDepthToLinear(depth);
    
    float4 baseColorSpecInt = colorSpecIntTexture.Load(location3);
    
    output.color = baseColorSpecInt.xyz;
    
    output.specInt = baseColorSpecInt.w;
    
    output.normal = normalTexture.Load(location3).xyz;
    output.normal = normalize(output.normal * 2.0f - 1.0f);
    
    float specPowerNormal = specPowTexture.Load(location3).x;
    output.specPow = specPowerRange.x + specPowerNormal * specPowerRange.y;
    
    return output;
}

struct PixelInput
{
    float4 pos : SV_Position;
    float2 uv : UV;
};

float3 CalcWorldPos(float2 csPos, float linearDepth)
{
    float4 position;

    float2 temp;
    temp.x = 1 / perspective[0][0];
    temp.y = 1 / perspective[1][1];
    position.xy = csPos.xy * temp * linearDepth;
    position.z = linearDepth;
    position.w = 1.0;
	
    return mul(position, viewInv).xyz;
}

float4 PS(PixelInput input) : SV_TARGET
{
	// Unpack the GBuffer
    SurfaceData gbd = UnpackGBuffer(input.pos.xy);
	
	// Convert the data into the material structure
    Material mat;
    mat.normal = gbd.normal;
    mat.diffuseColor.xyz = gbd.color;
    mat.diffuseColor.w = 1.0; // Fully opaque
    mat.specPow = specPowerRange.x + specPowerRange.y * gbd.specPow;
    mat.specIntensity = gbd.specInt;

	// Reconstruct the world position
    float3 position = CalcWorldPos(input.uv, gbd.linearDepth);

	// Calculate the ambient color
    float4 finalColor = 0;
    finalColor.xyz = CalcAmbient(mat.normal, mat.diffuseColor);

	// Calculate the directional light
    finalColor += CalcDirection(position, mat);

	// Return the final color
    return finalColor;
}