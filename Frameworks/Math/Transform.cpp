#include "Framework.h"

Transform::Transform(string tag)
	: tag(tag), position(0,0,0), rotation(0,0,0), scale(1,1,1),
	pivot(0,0,0), parent(nullptr)
{
	world = XMMatrixIdentity();
	worldBuffer = new MatrixBuffer();
}

Transform::~Transform()
{
	delete worldBuffer;
}

void Transform::UpdateWorld()
{
	world = XMMatrixTransformation(pivot.data, XMQuaternionIdentity(),
		scale.data, pivot.data, XMQuaternionRotationRollPitchYaw(rotation.x, rotation.y, rotation.z)
		, position.data);

	if (parent != nullptr)
		world *= *parent;

	worldBuffer->Set(world);
}

void Transform::SetWorldBuffer(UINT slot)
{
	worldBuffer->SetVSBuffer(slot);
}
