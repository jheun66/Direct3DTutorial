#pragma once

class ModelMesh
{
private:
	friend class Model;

	string name;

	int boneIndex;
	ModelBone* bone;

	vector<ModelMeshPart*> meshParts;

	Mesh* mesh;

	ModelVertex* vertices;
	UINT* indices;

	UINT vertexCount, indexCount;

	ModelMesh();
	~ModelMesh();

public:
	void CreateMesh();


	void Update();
	void Render();

	void SetTransforms(Matrix* transforms);

	int BoneIndex() { return boneIndex; }
};