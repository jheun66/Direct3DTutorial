#pragma once

class Quad : public Transform
{
private:
	VertexShader* vertexShader;
	PixelShader* pixelShader;
	VertexBuffer* vertexBuffer;
	IndexBuffer* indexBuffer;

	Texture* texture;
	ID3D11ShaderResourceView *srv;
	
	RasterizerState* rasterizerState[2];

public:
	Quad();
	~Quad();

	void Update();
	void Render();

	void SetTexure(wstring file);
	void SetSRV(ID3D11ShaderResourceView* srv) { this->srv = srv; }

private:
	void Create();
};