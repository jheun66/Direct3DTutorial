#include "Framework.h"

HullShader::HullShader(wstring file, string hsName)
{
    wstring path = L"Shaders/HullShaders/" + file + L".hlsl";
    // 컴파일 관련 기본 옵션
    DWORD flags = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG;

    V(D3DCompileFromFile(path.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
        hsName.c_str(), "hs_5_0", flags, 0, &hullBlob, nullptr));

    V(DEVICE->CreateHullShader(hullBlob->GetBufferPointer(),
        hullBlob->GetBufferSize(), nullptr, &hullShader));
}

HullShader::~HullShader()
{
    hullShader->Release();
    hullBlob->Release();
}

void HullShader::Set()
{
    DC->HSSetShader(hullShader, nullptr, 0);
}
