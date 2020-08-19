#include "Framework.h"

FreeCamera::FreeCamera()
	:moveSpeed(20.0f), rotSpeed(1.0f)
{
	oldPos = MOUSEPOS;
}

FreeCamera::~FreeCamera()
{

}

void FreeCamera::Update()
{
	Move();
	Rotation();

	View();
}

void FreeCamera::Move()
{
	if (KEY_PRESS(VK_RBUTTON))
	{
		if (KEY_PRESS('W'))
			position += forward * moveSpeed * DELTA;
		if (KEY_PRESS('S'))
			position -= forward * moveSpeed * DELTA;
		if (KEY_PRESS('A'))
			position -= right * moveSpeed * DELTA;
		if (KEY_PRESS('D'))
			position += right * moveSpeed * DELTA;
		if (KEY_PRESS('Q'))
			position += up * moveSpeed * DELTA;
		if (KEY_PRESS('E'))
			position -= up * moveSpeed * DELTA;
	}

	position += forward * Control::Get()->GetWheel() * moveSpeed * DELTA;

}

void FreeCamera::Rotation()
{
	Vector3 value = MOUSEPOS - oldPos;

	if (KEY_PRESS(VK_RBUTTON))
	{
		rotation.x += value.y * rotSpeed * DELTA;
		rotation.y += value.x * rotSpeed * DELTA;

		Camera::Rotation();
	}

	oldPos = MOUSEPOS;
}
