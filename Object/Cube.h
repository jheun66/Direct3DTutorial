#pragma once

class Cube : public Transform
{
private:
	VertexShader* vertexShader;
	PixelShader* pixelShader;
	VertexBuffer* vertexBuffer;
	IndexBuffer* indexBuffer;

	ColorBuffer* colorBuffer;

public:
	Cube();
	~Cube();

	void Update();
	void Render();

	void Create();

	void SetColor(Float4 color) { colorBuffer->data.color = color; }
};