#include "pixelHeader.hlsli"

struct PixelInput
{
    float4 pos : SV_Position;
};

//                          ¹ÝÈ¯°ªÀÇ ½Ã¸àÆ½ ³×ÀÓ(SV : system value)
float4 PS(PixelInput input) : SV_Target
{
    return mDiffuse;
}