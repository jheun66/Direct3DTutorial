#pragma once

class Material
{
public:
	class MaterialBuffer : public ConstBuffer
	{
	public:
		struct Data
		{
			Float4 diffuse;
			Float4 specular;
			Float4 ambient;
		}data;

		MaterialBuffer() : ConstBuffer(&data, sizeof(Data))
		{
			data.diffuse = Float4(1, 1, 1, 1);
			data.specular = Float4(1, 1, 1, 1);
			data.ambient = Float4(1, 1, 1, 1);
		}
	};

	string name;
private:
	MaterialBuffer* buffer;

	Texture* diffuseMap;
	Texture* specularMap;
	Texture* normalMap;

	VertexShader* vertexShader;
	PixelShader* pixelShader;

public:
	Material();
	Material(wstring file);
	Material(wstring vsFile, wstring psFile);
	Material(VertexShader* vertexShader, PixelShader* pixelShader);
	~Material();

	void Set();

	void SetShader(wstring file);

	void SetDiffuseMap(wstring file) { diffuseMap = Texture::Add(file); }
	void SetSpecularMap(wstring file) { specularMap = Texture::Add(file); }
	void SetNormalMap(wstring file) { normalMap = Texture::Add(file); }

	MaterialBuffer* GetBuffer() { return buffer; }
};