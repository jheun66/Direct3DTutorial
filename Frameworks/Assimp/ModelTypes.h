#pragma once

typedef VertexUVNormalTangent ModelVertex;


// ���ϴ°͸� ���� �̾Ƽ� ������
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

struct MeshData
{
	string name;
	string materialName;

	vector<ModelVertex> vertices;
	vector<UINT> indices;
};