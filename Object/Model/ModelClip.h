#pragma once

class ModelClip
{
private:
	friend class ModelAnimator;

	string name;

	float duration;
	float frameRate;
	UINT frameCount;

	unordered_map<string, KeyFrame*> keyFrameMap;

public:
	ModelClip();
	~ModelClip();

	KeyFrame* GetKeyFrame(string name);
};