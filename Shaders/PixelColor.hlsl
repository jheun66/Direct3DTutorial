struct PixelInput
{
    float4 pos : SV_Position;
    float4 color : Color;
};

//                          ��ȯ���� �ø�ƽ ����(SV : system value)
float4 PS(PixelInput input) : SV_Target
{
    return input.color;
}