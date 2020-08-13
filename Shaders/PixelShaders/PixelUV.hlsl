Texture2D map : register(t0);
SamplerState samp : register(s0);

struct PixelInput
{
    float4 pos : SV_Position;
    float2 uv : UV;
};

//                          ��ȯ���� �ø�ƽ ����(SV : system value)
float4 PS(PixelInput input) : SV_Target
{
    return map.Sample(samp, input.uv);
}