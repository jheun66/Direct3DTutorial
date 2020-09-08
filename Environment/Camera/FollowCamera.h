#pragma once

class FollowCamera : public Camera
{
private:
	// xz축 기준
	float distance;
	// y축 기준
	float height;

	Vector3 offset;
	float rotOffset;

	Vector3 destPos;
	float destRot;

	float moveDamping;
	float rotDamping;

	float rotY;
	float rotSpeed;

	float zoomSpeed;

	Transform* target;

public:
	FollowCamera();
	~FollowCamera();

	void Update() override;
	void Move() override;

	void MouseControl();

	void PostRender() override;

	void SetTarget(Transform* value) override { target = value; }
};