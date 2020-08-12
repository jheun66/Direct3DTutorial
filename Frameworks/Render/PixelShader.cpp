#include "Framework.h"

PixelShader::PixelShader(wstring file, string psName)
{
    // 컴파일 관련 기본 옵션
    DWORD flags = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG;

    V(D3DCompileFromFile(file.c_str(), nullptr, nullptr,
        psName.c_str(), "ps_5_0", flags, 0, &pixelBlob, nullptr));

    V(DEVICE->CreatePixelShader(pixelBlob->GetBufferPointer(),
        pixelBlob->GetBufferSize(), nullptr, &pixelShader));
}

PixelShader::~PixelShader()
{
    pixelShader->Release();
    pixelBlob->Release();
}

void PixelShader::Set()
{
    DC->PSSetShader(pixelShader, nullptr, 0);
}
