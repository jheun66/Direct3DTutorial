#pragma once

class Quad : public Transform
{
private:
	VertexShader* vertexShader;
	PixelShader* pixelShader;
	VertexBuffer* vertexBuffer;
	IndexBuffer* indexBuffer;

	ID3D11ShaderResourceView* srv;
	
	RasterizerState* rasterizerState[2];

public:
	Quad();
	~Quad();

	void Update();
	void Render();

	void Create();


};