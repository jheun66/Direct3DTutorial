#pragma once

class LightScene : public Scene
{
private:
	class PointBuffer : public ConstBuffer
	{
	public:
		struct Data
		{
			Float3 position;
			float range;
			Float4 color;
		}data;

		PointBuffer() : ConstBuffer(&data, sizeof(Data))
		{
			data.position = { 0,0,0 };
			data.range = 20.0f;
			data.color = { 1,1,1,1 };
		}
	};

	class SpotBuffer : public ConstBuffer
	{
	public:
		struct Data
		{
			Float3 position;
			float range;
			Float4 color;
			Float3 direction;
			float outer;
			float inner;

			float padding[3];
		}data;

		SpotBuffer() : ConstBuffer(&data, sizeof(Data))
		{
			data.position = { 0,0,0 };
			data.range = 20.0f;
			data.color = { 1,1,1,1 };
			data.direction = { 0,-1,0 };
			data.outer = 65;
			data.inner = 55;
		}
	};

	ModelRender* bunny;
	ModelRender* plane;
	ModelRender* door;

	//PointBuffer* pointBuffer;
	//SpotBuffer* spotBuffer;

	LightInfoBuffer* lightBuffer;
public:
	LightScene();
	~LightScene();

	// Scene을(를) 통해 상속됨
	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;

	void Export(string name);
};