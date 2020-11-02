#pragma once

class ModelAnimator : public Model
{
protected:
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

	struct TweenDesc
	{
		float takeTime;
		float tweenTime;
		float runningTime;
		float padding;

		KeyFrameDesc cur;
		KeyFrameDesc next;

		TweenDesc()
		{
			takeTime = 0.5f;
			tweenTime = 0.0f;
			runningTime = 0.0f;

			cur.clip = 0;
			next.clip = 1;
		}

	};


	class FrameBuffer : public ConstBuffer
	{
	public:
		struct Data
		{
			TweenDesc tweenDesc[MAX_MODEL_INSTANCE];
		}data;

		FrameBuffer() : ConstBuffer(&data, sizeof(Data))
		{
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
	// shader에서 정점 보간용
	ClipTransform* clipTransform;
	// 본 위치 찾는용도
	ClipTransform* nodeTransform;

	ID3D11Texture2D* texture;
	ID3D11ShaderResourceView* srv;

	vector<ModelClip*> clips;

	vector<Transform*> transforms;
	Matrix worlds[MAX_MODEL_INSTANCE];

	VertexBuffer* instanceBuffer;

	map<UINT, function<void()>> EndEvent;

	Frustum* frustum;
	UINT frustumCount;
public:
	ModelAnimator(string file);
	virtual ~ModelAnimator();

	void Update();
	void Render();
	void PostRender();

	void UpdateTransforms();

	Transform* AddTransform();

	void PlayClip(UINT instance, UINT clip, float speed = 1.0f, float takeTime = 0.2f);
	void ReadClip(string file);

	void SetEndEvent(UINT clip, function<void()> value) { EndEvent[clip] = value; }

	Matrix GetCurNodeMatrix(UINT nodeIndex);
private:
	void CreateTexture();
	void CreateClipTransform(UINT index);
};