#include "Framework.h"

DomainShader::DomainShader(wstring file, string dsName)
{
    wstring path = L"Shaders/DomainShaders/" + file + L".hlsl";
    // 컴파일 관련 기본 옵션
    DWORD flags = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG;

    V(D3DCompileFromFile(path.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
        dsName.c_str(), "ds_5_0", flags, 0, &domainBlob, nullptr));

    V(DEVICE->CreateDomainShader(domainBlob->GetBufferPointer(),
        domainBlob->GetBufferSize(), nullptr, &domainShader));
}

DomainShader::~DomainShader()
{
    domainShader->Release();
    domainBlob->Release();
}

void DomainShader::Set()
{
    DC->DSSetShader(domainShader, nullptr, 0);
}
