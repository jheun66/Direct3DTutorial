#pragma once

class FreeCamera : public Camera
{
private:
	float moveSpeed;
	float rotSpeed;

	Vector3 oldPos;
public:
	FreeCamera();
	~FreeCamera();

	void Update() override;

private:
	void Move() override;
	void Rotation() override;
};