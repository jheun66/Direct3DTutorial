Texture2D map : register(t0);
SamplerState samp : register(s0);

struct PixelInput
{
    float4 pos : SV_Position;
    float2 uv : UV;
    float diffuse : DIFFUSE;
};

//                          ¹ÝÈ¯°ªÀÇ ½Ã¸àÆ½ ³×ÀÓ(SV : system value)
float4 PS(PixelInput input) : SV_Target
{
    float4 albedo = map.Sample(samp, input.uv);
    
    // °£Á¢±¤
    float4 ambient = albedo * 0.1f;
    
    return albedo * input.diffuse + ambient;
}