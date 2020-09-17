#pragma once

class Spark : public Particle
{
private:
	struct VertexParticle
	{
		Float3 position;
		Float2 size;
		Float3 velocity;
	};

	class SparkBuffer :public ConstBuffer
	{
	public:
		struct Data
		{
			float duration;
			float time;
			float gravity;

			float padding;
		}data;

		SparkBuffer() : ConstBuffer(&data, sizeof(Data))
		{
			data.duration = 0.0f;
			data.time = 0.0f;
			data.gravity = 0.0f;
		}

	};

	SparkBuffer* sparkBuffer;
	VertexParticle* vertices;

public:
	Spark();
	~Spark();


	// Particle을(를) 통해 상속됨
	virtual void Create() override;
	virtual void Update() override;
	virtual void Render() override;

	virtual void Play(Vector3 position) override;
};