#include "Framework.h"

Camera::Camera()
	: position(0, 1, -5), rotation(0, 0, 0),
	forward(0, 0, 1), right(1, 0, 0), up(0, 1, 0)
{
	matRotation = XMMatrixIdentity();
	matView = XMMatrixIdentity();
	
	viewBuffer = new ViewBuffer();
	
	oldPos = MOUSEPOS;
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

void Camera::PostRender()
{
	ImGui::Text("CamPos : %.2f, %.2f, %.2f", position.x, position.y, position.z);
	ImGui::Text("CamRot : %.2f, %.2f, %.2f", rotation.x, rotation.y, rotation.z);
}

void Camera::VSSet(UINT slot)
{
	viewBuffer->SetVSBuffer(slot);
}

void Camera::GSSet(UINT slot)
{
	viewBuffer->SetGSBuffer(slot);
}

Ray Camera::ScreenPointToRay(Vector3 pos)
{
	Float2 screenSize(WIN_WIDTH, WIN_HEIGHT);

	Float2 point;
	point.x = ((2 * pos.x) / screenSize.x) - 1.0f;
	point.y = (((2 * pos.y) / screenSize.y) - 1.0f) * -1.0f;

	Matrix projection = Environment::Get()->GetPerspective();

	Float4x4 temp;
	XMStoreFloat4x4(&temp, projection);

	point.x /= temp._11;
	point.y /= temp._22;

	Ray ray;
	ray.position = position;

	Matrix invView = XMMatrixInverse(nullptr, matView);

	Vector3 tempPos(point.x, point.y, 1.0f);

	ray.direction = XMVector3TransformNormal(tempPos.data, invView);
	ray.direction.Normalize();

	return ray;
}
