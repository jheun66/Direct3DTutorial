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

	ModelRender* bunny;
	ModelRender* plane;

	PointBuffer* pointBuffer;
public:
	LightScene();
	~LightScene();

	// Scene��(��) ���� ��ӵ�
	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;

	void Export(string name);
};