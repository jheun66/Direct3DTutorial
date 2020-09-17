#pragma once

class GeometryShader : public Shader
{
private:
	friend class Shader;

	ID3D11GeometryShader* geometryShader;

	ID3DBlob* geometryBlob;

	GeometryShader(wstring file, string gsName = "GS");
	~GeometryShader();

public:

	void Set() override;
};
