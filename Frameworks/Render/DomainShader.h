#pragma once

class DomainShader : public Shader
{
private:
	friend class Shader;

	ID3D11DomainShader* domainShader;

	ID3DBlob* domainBlob;

	DomainShader(wstring file, string dsName = "DS");
	~DomainShader();

public:

	void Set() override;
};