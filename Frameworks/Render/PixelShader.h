#pragma once

class PixelShader :public Shader
{
private:
	friend class Shader;

	ID3D11PixelShader* pixelShader;

	ID3DBlob* pixelBlob;

	PixelShader(wstring file, string psName = "PS");
	~PixelShader();
public:

	void Set() override;
};