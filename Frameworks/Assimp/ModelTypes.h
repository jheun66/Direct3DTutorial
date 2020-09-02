#pragma once

typedef VertexUVNormalTangent ModelVertex;


// 원하는것만 따로 뽑아서 쓰도록
struct MaterialData
{
	string name;

	Float4 ambient;
	Float4 diffuse;
	Float4 specular;

	string diffuseFile;
	string specularFile;
	string normalFile;
};

struct BoneData
{
	int index;
	string name;

	int parent;
	Matrix transform;
};

struct MeshPartData
{
	string name;
	string materialName;

	UINT startVertex;
	UINT vertexCount;

	UINT startIndex;
	UINT indexCount;
};

struct MeshData
{
	string name;
	int boneIndex;

	aiMesh* mesh;

	vector<ModelVertex> vertices;
	vector<UINT> indices;

	vector<MeshPartData*> meshParts;
};