#pragma once

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

	MatrixBuffer* viewBuffer;

public:
	Camera();
	virtual ~Camera();

	virtual void Update();
	virtual void Move();
	virtual void Rotation();
	virtual void View();

	void VSSet(UINT slot = 1);


	Matrix GetView() { return matView; }

};