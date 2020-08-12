#pragma once

class Cube
{
private:
	Float3 position;
	Float3 rotation;
	Float3 scale;

	Float3 pivot;

	Matrix world;
	Matrix* parent;

	VertexShader* vertexShader;
	PixelShader* pixelShader;
	VertexBuffer* vertexBuffer;
	IndexBuffer* indexBuffer;

	MatrixBuffer* worldBuffer;

	bool isControl = false;

public:
	Cube();
	~Cube();

	void Update();
	void Render();

	void Create();

	Matrix* GetWorld() { return &world; }
	void SetPos(Float3 value) { position = value; }
	void SetControl(bool value) { isControl = value; }
	void SetParent(Matrix* value) { parent = value; }
};