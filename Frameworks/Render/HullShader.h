#pragma once

class HullShader : public Shader
{
private:
	friend class Shader;

	ID3D11HullShader* hullShader;

	ID3DBlob* hullBlob;

	HullShader(wstring file, string hsName = "HS");
	~HullShader();

public:

	void Set() override;
};