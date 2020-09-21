#pragma once

class Spark : public Particle
{
private:

	struct VertexParticle
	{
		Float3 position;
		Float2 size;
		Float3 velocity;

		VertexParticle()
			:position(0,0,0), size(0,0), velocity(0,0,0)
		{

		}

	};

	class SparkBuffer :public ConstBuffer
	{
	public:
		struct Data
		{
			Float3 direction;
			float duration;

			float time;
			float padding[3];

		}data;

		SparkBuffer() : ConstBuffer(&data, sizeof(Data))
		{
			data.direction = { 0,0,0 };
			data.duration = 0.0f;
			data.time = 0.0f;
		}

	};

	SparkBuffer* sparkBuffer;
	VertexParticle* vertices;

	ColorBuffer* startColorBuffer;
	ColorBuffer* endColorBuffer;

public:
	Spark();
	~Spark();


	// Particle을(를) 통해 상속됨
	virtual void Create() override;
	virtual void Update() override;
	virtual void Render() override;
	virtual void PostRender() override;


	virtual void Play(Vector3 position) override;

	void UpdateParticle();
};