#pragma once

class Snow : public Particle
{
private:
	class SnowBuffer : public ConstBuffer
	{
	public:
		struct Data
		{
			Float3 velocity;
			float distance;

			Float4 color;

			Float3 origin;
			float time;

			Float3 size;
			float turbulence;
		}data;

		SnowBuffer() : ConstBuffer(&data, sizeof(Data))
		{
			data.velocity = Float3(0, -5, 0);
			data.distance = 50.0f;

			data.color = Float4(1, 1, 1, 1);

			data.origin = Float3(0, 0, 0);
			data.size = Float3(10, 10, 10);

			data.time = 0.0f;
			data.turbulence = 5.0f;
		}
	};

	struct VertexSnow
	{
		Float3 position;
		float scale;
		Float2 random;
	};

	SnowBuffer* snowBuffer;
	VertexSnow* vertices;

public:
	Snow();
	~Snow();

	// Particle을(를) 통해 상속됨
	virtual void Create() override;
	virtual void Update() override;
	virtual void Render() override;
	virtual void PostRender() override;

};