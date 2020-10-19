#pragma once

class Water : public Transform
{
private:
	class WaterBuffer : public ConstBuffer
	{
	public:
		struct Data
		{
			Float4 color;

			float waveTranslation;
			float waveScale;
			float shininess;
			float alpha;

			float waveSpeed;
			float padding[3];
			
			Data()
			{
				color = Float4(0.2f, 0.3f, 1.0f, 1.0f);

				waveTranslation = 0.0f;
				waveScale = 0.05f;
				shininess = 30.0f;
				alpha = 0.5f;

				waveSpeed = 0.05f;
			}
		}data;

		WaterBuffer() : ConstBuffer(&data, sizeof(Data))
		{
		}
	};

	WaterBuffer* buffer;
	float radius;
	
	Material* material;
	Mesh* mesh;

	UINT vertexCount, indexCount;

	Reflection* reflection;
	Refraction* refraction;

	BlendState* blendState[2];

public:
	Water(float radius = 30.0f);
	~Water();

	void Update();

	void SetReflection();
	void SetRefraction();
	void Render();
	void PostRender();

private:
	void CreateMesh();
};