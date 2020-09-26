#pragma once

class SpriteEffect : public Particle
{
private:
	class SpriteBuffer : public ConstBuffer
	{
	public:
		struct Data
		{
			Float2 maxFrame;
			Float2 curFrame;
		}data;
	
		SpriteBuffer() : ConstBuffer(&data, sizeof(Data))
		{
			data.maxFrame = {0,0};
			data.curFrame = {0,0};
		}
	};

	SpriteBuffer* spriteBuffer;
	VertexSize* vertices;

	float time;
	float speed;
	UINT curFrameCount;
	UINT maxFrameCount;

	wstring diffuseFile;

public:
	SpriteEffect(wstring diffuseFile, Float2 maxFrame);
	~SpriteEffect();

	// Particle을(를) 통해 상속됨
	virtual void Create() override;
	virtual void Update() override;
	virtual void Render() override;
	virtual void PostRender() override;

	virtual void Play(Vector3 position) override;

};