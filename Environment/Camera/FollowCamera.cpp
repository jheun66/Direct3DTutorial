#include "Framework.h"

FollowCamera::FollowCamera()
	:distance(60), height(60), offset(0,5,0), 
	moveDamping(5), rotDamping(0), destPos(0,0,0), destRot(0), 
	rotY(0), rotSpeed(0.001f), zoomSpeed(0.1f),
	target(nullptr)
{

}

FollowCamera::~FollowCamera()
{
}

void FollowCamera::Update()
{
	if (target == nullptr)
		return;

	Move();
}

void FollowCamera::Move()
{
	//Vector3 tempPos = Vector3(0, 0, -distance);

	if (rotDamping > 0.0f)
	{
		if (target->rotation.y != destRot)
		{
			destRot = LERP(destRot, target->rotation.y, rotDamping * DELTA);
		}

		matRotation = XMMatrixRotationY(destRot);
	}
	else
	{
		MouseControl();

		matRotation = XMMatrixRotationY(rotY);
	}

	forward = XMVector3TransformNormal(kForward, matRotation);
	destPos = forward * -distance;

	destPos += target->WorldPos();
	destPos.y += height;

	position = XMVectorLerp(position.data, destPos.data, moveDamping * DELTA);

	Vector3 tempOffset = XMVector3TransformCoord(offset.data, matRotation);

	matView = XMMatrixLookAtLH(position.data, (target->WorldPos() + tempOffset).data, up.data);

	viewBuffer->Set(matView);
}

void FollowCamera::MouseControl()
{
	if (KEY_PRESS(VK_RBUTTON))
	{
		Vector3 value = MOUSEPOS - oldPos;

		rotY += value.x * rotSpeed;
	}

	oldPos = MOUSEPOS;

	distance -= Control::Get()->GetWheel() * zoomSpeed;
	height -= Control::Get()->GetWheel() * zoomSpeed;

	//if (distance < 1.0f)
	//	distance = 1.0f;

	//if (height < 1.0f)
	//	height = 1.0f;
}

void FollowCamera::PostRender()
{
	Camera::PostRender();

	ImGui::SliderFloat("CamDistance", &distance, -10, 100);
	ImGui::SliderFloat("CamHeight", &height, 1, 100);
	ImGui::SliderFloat("CamMoveDamping", &moveDamping, 0, 30);
	ImGui::SliderFloat("CamRotDamping", &rotDamping, 0, 30);
	ImGui::SliderFloat3("CamOffset", (float*)&offset, -20.0f, 20.0f);
}
