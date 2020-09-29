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
			float takeTime;
			float tweenTime;
			float runningTime;
			float padding;

			KeyFrameDesc cur;
			KeyFrameDesc next;
		}data;

		FrameBuffer() : ConstBuffer(&data, sizeof(Data))
		{
			data.takeTime = 0.0f;
			data.tweenTime = 0.0f;
			data.runningTime = 0.0f;
		}
	};

	struct ClipTransform
	{
		// Matrix 의 2차원 배열
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

	map<UINT, function<void()>> EndEvent;
public:
	ModelAnimator(string file);
	virtual ~ModelAnimator();

	void Update();
	void Render();

	void PlayClip(UINT clip, float speed = 1.0f, float takeTime = 0.2f);
	void ReadClip(string file);

	void SetEndEvent(UINT clip, function<void()> value) { EndEvent[clip] = value; }

	Matrix GetCurBoneMatrix(UINT boneIndex);
private:
	void CreateTexture();
	void CreateClipTransform(UINT index);
};