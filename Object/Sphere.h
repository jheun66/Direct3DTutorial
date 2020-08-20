#pragma once

class Sphere : public Transform
{
private:
	typedef VertexUVNormal VertexType;

	// Material
	Texture* texture;
	VertexShader* vertexShader;
	PixelShader* pixelShader;

	// Mesh
	VertexBuffer* vertexBuffer;
	IndexBuffer* indexBuffer;
	vector<VertexType> vertices;
	vector<UINT> indices;

	UINT sliceCount;
	UINT stackCount;
	float radius;

	RasterizerState* fillMode[2];

public:
	Sphere(float radius =1.0f, UINT sliceCount = 20, UINT stackCount = 20);
	~Sphere();

	void Update();
	void Render();

private:
	void CreateMesh();

};