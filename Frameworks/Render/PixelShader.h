#pragma once

class PixelShader
{
private:
	ID3D11PixelShader* pixelShader;

	ID3DBlob* pixelBlob;

public:
	PixelShader(wstring file, string psName = "PS");
	~PixelShader();

	void Set();
};