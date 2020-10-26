#pragma once

typedef VertexUVNormalTangentBlend ModelVertex;


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

struct NodeData
{
	int index;
	string name;
	int parent;
	Float4x4 transform;
};

struct BoneData
{
	string name;
	int index;
	Float4x4 offset;
};


struct MeshData
{
	string name;
	string materialName;

	vector<ModelVertex> vertices;
	vector<UINT> indices;
};

struct VertexWeights
{
	UINT indices[4];
	float weights[4];

	VertexWeights() : indices{}, weights{}
	{
	}

	void Add(const UINT& index, const float& weight)
	{
		for (UINT i = 0; i < 4; i++)
		{
			if (weights[i] == 0.0f)
			{
				indices[i] = index;
				weights[i] = weight;
				return;
			}
		}
	}

	void Normalize()
	{
		float sum = 0.0f;
		for (UINT i = 0; i < 4; i++)
			sum += weights[i];

		for (UINT i = 0; i < 4; i++)
			weights[i] /= sum;
	}
};


struct KeyTransform
{
	float time;
	
	Float3 scale;
	
	// ���ʹϾ����� ���
	Float4 rotation;
	Float3 position;
};

struct KeyFrame
{
	string boneName;
	vector<KeyTransform> transforms;
};

struct ClipNode
{
	vector<KeyTransform> keyFrame;
	aiString name;
};

struct Clip
{
	string name;

	UINT frameCount;
	// �ӵ�
	float frameRate;
	// ���� �ð�
	float duration;

	vector<KeyFrame*> keyFrame;
};

