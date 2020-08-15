#pragma once

class Quad : public Transform
{
private:
	VertexShader* vertexShader;
	PixelShader* pixelShader;
	VertexBuffer* vertexBuffer;
	IndexBuffer* indexBuffer;

	Texture* texture;
	
	RasterizerState* rasterizerState[2];

public:
	Quad();
	~Quad();

	void Update();
	void Render();

	void Create();


};