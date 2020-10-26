#pragma once

class ModelMesh
{
private:
	friend class Model;

	string name;

	string materialName;
	Material* material;

	Mesh* mesh;

	ModelVertex* vertices;
	UINT* indices;

	UINT vertexCount, indexCount;

	ModelMesh();
	~ModelMesh();

public:
	void CreateMesh();

	void Update();
	void Render(UINT drawCount = 1);
};