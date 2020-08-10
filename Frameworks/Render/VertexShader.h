#pragma once

class VertexShader
{
private:
	ID3D11VertexShader* vertexShader;
	ID3D11InputLayout* inputLayout;

	ID3DBlob* vertexBlob;

	// hlsl ���Ͽ� �����ؼ� �ʿ��� ���� �޾ƿ�
	ID3D11ShaderReflection* reflection;

public:
	VertexShader(wstring file, string vsName = "VS");
	~VertexShader();

	void Set();

private:
	void CreateInputLayout();
};