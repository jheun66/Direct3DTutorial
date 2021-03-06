#include "PixelHeader.hlsli"

struct PixelInput
{
    float4 pos : SV_Position;
    float2 uv : UV;
};

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
    finalColor += CalcDirection(position, mat, lights[0]);

	// Return the final color
    return finalColor;
}