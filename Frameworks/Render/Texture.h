#pragma once

class Texture
{
private:
	ScratchImage image;

	ID3D11ShaderResourceView* srv;

	static map<wstring, Texture*> totalTexture;

	Texture(ID3D11ShaderResourceView* srv, ScratchImage& image);
	~Texture();

public:
	static Texture* Add(wstring file);
	static void Delete();

	void PSSet(UINT slot);

	vector<Float4> ReadPixels();

	UINT GetWidth() { return image.GetMetadata().width; }
	UINT GetHeight() { return image.GetMetadata().height; }
};