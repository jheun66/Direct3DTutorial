#pragma once

struct Ray
{
	Vector3 position;
	Vector3 direction;
};

class Camera
{
protected:
	Vector3 position;
	Vector3 rotation;

	Vector3 forward;
	Vector3 right;
	Vector3 up;

	Matrix matRotation;
	Matrix matView;

	ViewBuffer* viewBuffer;

public:
	Camera();
	virtual ~Camera();

	virtual void Update();
	virtual void Move();
	virtual void Rotation();
	virtual void View();

	void VSSet(UINT slot = 1);

	Ray ScreenPointToRay(Vector3 pos);

	Matrix GetView() { return matView; }
	Vector3 GetPos() { return position; }
};