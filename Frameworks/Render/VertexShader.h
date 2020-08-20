#pragma once

class VertexShader : public Shader
{
private:
	friend class Shader;

	ID3D11VertexShader* vertexShader;
	ID3D11InputLayout* inputLayout;

	ID3DBlob* vertexBlob;

	// hlsl 파일에 접근해서 필요한 정보 받아옴
	ID3D11ShaderReflection* reflection;

	VertexShader(wstring file, string vsName = "VS");
	~VertexShader();
public:

	void Set() override; 

private:
	void CreateInputLayout();
};