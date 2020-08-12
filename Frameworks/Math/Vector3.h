#pragma once

class Vector3
{
public:
	Vector4 data;

	Vector3();
	Vector3(Float3 value);
	Vector3(float x,float y, float z);
	Vector3(Vector4 value);

	operator Float3();
};