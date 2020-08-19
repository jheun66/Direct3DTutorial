#include "Framework.h"

Camera::Camera()
	: position(0, 0, 0), rotation(0, 0, 0),
	forward(0, 0, 1), right(1, 0, 0), up(0, 1, 0)
{
	matRotation = XMMatrixIdentity();
	matView = XMMatrixIdentity();
	
	viewBuffer = new MatrixBuffer();
}

Camera::~Camera()
{
	delete viewBuffer;
}

void Camera::Update()
{
}

void Camera::Move()
{
}

void Camera::Rotation()
{
	Matrix rotX = XMMatrixRotationX(rotation.x);
	Matrix rotY = XMMatrixRotationY(rotation.y);

	matRotation = rotX * rotY;

	right = XMVector3TransformNormal(kRight, matRotation);
	up = XMVector3TransformNormal(kUp, matRotation);
	forward = XMVector3TransformNormal(kForward, matRotation);
}


void Camera::View()
{
	Vector3 focus = position + forward;
	matView = XMMatrixLookAtLH(position.data, focus.data, up.data);
	viewBuffer->Set(matView);
}

void Camera::VSSet(UINT slot)
{
	viewBuffer->SetVSBuffer(slot);
}
