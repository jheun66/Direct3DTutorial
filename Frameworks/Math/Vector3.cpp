#include "Framework.h"

Vector3::Vector3()
{
	data = XMVectorSet(0, 0, 0, 0);
}

Vector3::Vector3(Float3 value)
{
	data = XMLoadFloat3(&value);
}

Vector3::Vector3(float x, float y, float z)
{
	data = XMVectorSet(x, y, z, 0);
}

Vector3::Vector3(Vector4 value)
	: data(value)
{

}

Vector3::operator Float3()
{
	Float3 result;
	XMStoreFloat3(&result, data);

	return result;
}
