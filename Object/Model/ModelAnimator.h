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
			data.takeTime = 1.0f;
			data.tweenTime = 0.0f;
			data.runningTime = 0.0f;
		}
	};

};