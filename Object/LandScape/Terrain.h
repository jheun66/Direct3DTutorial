#pragma once

class Terrain : Transform
{
private:
	typedef VertexUVNormalTangent VertexType;

	struct InputDesc
	{
		UINT index;
		Float3 v0, v1, v2;
	};

	struct OutputDesc
	{
		UINT picked;
		float u, v, distance;
	};

	Material* material;
	Mesh* mesh;

	vector<VertexType> vertices;
	vector<UINT> indices;

	UINT width, height;

	Texture* heightMap;

	ComputeShader* computeShader;
	RayBuffer* rayBuffer;

	StructuredBuffer* structuredBuffer;

	InputDesc* input;
	OutputDesc* output;

	UINT size;

public:
	Terrain();
	~Terrain();

	void Update();
	void Render();

	bool Picking(OUT Vector3* position);
	float GetHeight(Vector3 position);
	
	bool ComputePicking(OUT Vector3* position);

	Float2 GetSize() { return Float2(width, height); }
	Material* GetMaterial() { return material; }
private:
	void CreateData();
	void CreateNormal();
	void CreateTangent();


};