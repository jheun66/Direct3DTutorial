#include "pixelHeader.hlsli"

struct PixelInput
{
    float4 pos : SV_Position;
};

//                          ��ȯ���� �ø�ƽ ����(SV : system value)
float4 PS(PixelInput input) : SV_Target
{
    return mDiffuse;
}