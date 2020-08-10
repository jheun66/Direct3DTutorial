#include "Framework.h"

VertexShader::VertexShader(wstring file, string vsName)
{
    // 컴파일 관련 기본 옵션
    DWORD flags = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG;

    V(D3DCompileFromFile(file.c_str(), nullptr, nullptr,
        vsName.c_str(), "vs_5_0", flags, 0, &vertexBlob, nullptr));

    V(DEVICE->CreateVertexShader(vertexBlob->GetBufferPointer(),
        vertexBlob->GetBufferSize(), nullptr, &vertexShader));

    CreateInputLayout();

}

VertexShader::~VertexShader()
{
    vertexShader->Release();
    vertexBlob->Release();

    inputLayout->Release();
}

void VertexShader::Set()
{
    DC->IASetInputLayout(inputLayout);
    DC->VSSetShader(vertexShader, nullptr, 0);
}

void VertexShader::CreateInputLayout()
{
    D3DReflect(vertexBlob->GetBufferPointer(), vertexBlob->GetBufferSize(),
        IID_ID3D11ShaderReflection, (void**)&reflection);

    D3D11_SHADER_DESC shaderDesc;
    reflection->GetDesc(&shaderDesc);

    vector<D3D11_INPUT_ELEMENT_DESC> inputLayouts;

    for (UINT i = 0; i < shaderDesc.InputParameters; i++)
    {
        D3D11_SIGNATURE_PARAMETER_DESC paramDesc;

        reflection->GetInputParameterDesc(i, &paramDesc);

        D3D11_INPUT_ELEMENT_DESC elementDesc;


    }

}
