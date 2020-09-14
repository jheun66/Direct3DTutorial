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

	XMMatrixDecompose(&globalScale.data, &globalRotation.data, 
		&globalPosition.data, world);

	worldBuffer->Set(world);
}

void Transform::SetWorldBuffer(UINT slot)
{
	worldBuffer->SetVSBuffer(slot);
}

Vector3 Transform::Forward()
{
	return XMVector3Normalize(XMVector3TransformNormal(kForward, world));
}

Vector3 Transform::Up()
{
	return XMVector3Normalize(XMVector3TransformNormal(kUp, world));
}

Vector3 Transform::Right()
{
	// 마지막 w 0
	return XMVector3Normalize(XMVector3TransformNormal(kRight, world));
}

Vector3 Transform::WorldPos()
{
	// 마지막 w 1
	//return XMVector3TransformCoord(XMVectorZero(), world);
	return globalPosition;
}

Vector3 Transform::WorldRot()
{
	float q0 = globalRotation.w;
	float q1 = globalRotation.x;
	float q2 = globalRotation.y;
	float q3 = globalRotation.z;
	
	Vector3 rot;
	rot.x = atan2(2 * (q0 * q1 + q2 * q3), 1 - 2 * (q1 * q1 + q2 * q2));
	
	
	float sinp= asin(2.0f * (q0 * q2 - q3 * q1));
	if (std::abs(sinp) >= 1)
		rot.y = std::copysign(XM_PI / 2, sinp); // use 90 degrees if out of range
	else
		rot.y = std::asin(sinp);

	float siny_cosp = 2 * (q0 * q3 + q1 * q2);
	float cosy_cosp = 1 - 2 * (q2 * q2 + q3 * q3);
	rot.z = atan2(siny_cosp, cosy_cosp);


	return rot;
	//return globalRotation.data;
}

Vector3 Transform::WorldScale()
{
	return globalScale;
}
