#pragma once

class TerrainEditor : public Transform
{
public:
	class BrushBuffer : public ConstBuffer
	{
	public:
		struct Data
		{
			int type;
			Float3 location;

			float range;
			Float3 color;
		}data;
		BrushBuffer() : ConstBuffer(&data, sizeof(Data))
		{
			data.type = 1;
			data.location = Float3(0, 0, 0);

			data.range = 10.0f;
			data.color = Float3(0, 1, 0);
		}

	};

private:
	typedef VertexUVNormalAlpha VertexType;

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

	ComputeShader* computeShader;
	RayBuffer* rayBuffer;

	StructuredBuffer* structuredBuffer;

	InputDesc* input;
	OutputDesc* output;

	UINT size;

	BrushBuffer* brushBuffer;

	bool isRaise;
	float adjustValue;

	bool isPainting;
	float paintValue;

	int selectMap;

	vector<float> heights;

	//Texture* alphaMap;
	Texture* secondMap;
	Texture* thirdMap;

public:
	TerrainEditor(UINT width, UINT height);
	~TerrainEditor();

	void Update();
	void Render();
	void PostRender();

	bool ComputePicking(OUT Vector3* position);

	void AdjustY(Vector3 position, float value);
	void PaintBrush(Vector3 position, float value);

	void Save();
	void Load();

	void SaveHeightMap();
	void SaveAlphaMap();

private:
	void CreateData();
	void CreateNormal();
	void CreateCompute();

};