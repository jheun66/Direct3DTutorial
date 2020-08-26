#pragma once

class ComputeShader : public Shader
{
private:
	friend class Shader;

	ID3D11ComputeShader* shader;
	ID3DBlob* computeBlob;

	ComputeShader(wstring file, string csName = "CS");
	~ComputeShader();

public:

	// Shader을(를) 통해 상속됨
	virtual void Set() override;




};