#pragma once

class ModelAnimator : public Model
{
private:
	struct KeyFrameDesc
	{
		int clip = 0;

		UINT curFrame = 0;
		UINT nextFrame = 0;

		float time = 0.0f;
		float runningTime = 0.0f;

		float speed = 1.0f;

		float padding[2];
	};

	class FrameBuffer : public ConstBuffer
	{
	public:
		struct Data
		{
			KeyFrameDesc keyFrame;
		}data;

		FrameBuffer() : ConstBuffer(&data, sizeof(Data))
		{
		}
	};

	struct ClipTransform
	{
		// Matrix �� 2���� �迭
		Matrix** transform;
		
		ClipTransform()
		{
			transform = new Matrix * [MAX_ANIM_KEY];

			for (UINT i = 0; i < MAX_ANIM_KEY; i++)
				transform[i] = new Matrix[MAX_MODEL_BONE];
		}
		~ClipTransform()
		{
			for (UINT i = 0; i < MAX_ANIM_KEY; i++)
				delete transform[i];

			delete[] transform;
		}
	
	};

	FrameBuffer* frameBuffer;
	ClipTransform* clipTransform;

	ID3D11Texture2D* texture;
	ID3D11ShaderResourceView* srv;

	vector<ModelClip*> clips;
public:
	ModelAnimator(string file);
	~ModelAnimator();

	void Update();
	void Render();

	void PlayClip(UINT clip, float speed = 1.0f);
	void ReadClip(string file);

private:
	void CreateTexture();
	void CreateClipTransform(UINT index);
};