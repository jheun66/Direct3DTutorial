#pragma once

class VertexShader
{
private:
	ID3D11VertexShader* vertexShader;
	ID3D11InputLayout* inputLayout;

	ID3DBlob* vertexBlob;

	// hlsl 파일에 접근해서 필요한 정보 받아옴
	ID3D11ShaderReflection* reflection;

public:
	VertexShader(wstring file, string vsName = "VS");
	~VertexShader();

	void Set();

private:
	void CreateInputLayout();
};