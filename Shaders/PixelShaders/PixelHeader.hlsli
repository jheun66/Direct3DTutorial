Texture2D diffuseMap : register(t0);
Texture2D specularMap : register(t1);
Texture2D normalMap : register(t2);

SamplerState samp : register(s0);

cbuffer Light : register(b0)
{
    float3 lightDir;
    float specExp;
    float4 ambient;
}

cbuffer Material : register(b1)
{
    float4 mDiffuse;
    float4 mSpecular;
    float4 mAmbient;
}