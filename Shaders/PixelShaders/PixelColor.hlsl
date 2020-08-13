struct PixelInput
{
    float4 pos : SV_Position;
    float4 color : Color;
};

//                          ¹ÝÈ¯°ªÀÇ ½Ã¸àÆ½ ³×ÀÓ(SV : system value)
float4 PS(PixelInput input) : SV_Target
{
    return input.color;
}