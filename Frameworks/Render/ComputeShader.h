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

	// Shader��(��) ���� ��ӵ�
	virtual void Set() override;




};