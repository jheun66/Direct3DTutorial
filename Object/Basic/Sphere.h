#pragma once

class Sphere : public Transform
{
private:
	typedef VertexUVNormalTangent VertexType;

	// Material
	Material* material;

	// Mesh
	Mesh* mesh;

	vector<VertexType> vertices;
	vector<UINT> indices;

	UINT sliceCount;
	UINT stackCount;
	float radius;

	RasterizerState* fillMode[2];

public:
	Sphere(float radius =1.0f, UINT sliceCount = 20, UINT stackCount = 20);
	Sphere(wstring shaderFile, float radius = 1.0f, UINT sliceCount = 20, UINT stackCount = 20);
	~Sphere();

	void Update();
	void Render();

	float GetRadius() { return radius; }
	Material* GetMaterial() { return material; }
	RasterizerState* GetFillMode() { return fillMode[1]; }

private:
	void CreateMesh();
	void CreateTangent();
};