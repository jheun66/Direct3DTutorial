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

	void SetX(float value);
	void SetY(float value);
	void SetZ(float value);
	float GetX();
	float GetY();
	float GetZ();

	__declspec(property(get = GetX, put = SetX)) float x;
	__declspec(property(get = GetY, put = SetY)) float y;
	__declspec(property(get = GetZ, put = SetZ)) float z;

	Vector3 operator+(const Vector3& value) const;
	Vector3 operator-(const Vector3& value) const;

	// 벡터 개념은 존재 x
	Vector3 operator*(const Vector3& value) const;
	Vector3 operator/(const Vector3& value) const;

	void operator+=(const Vector3& value);
	void operator-=(const Vector3& value);
	void operator*=(const Vector3& value);
	void operator/=(const Vector3& value);

	// 벡터 개념은 존재 x
	Vector3 operator+(const float& value) const;
	Vector3 operator-(const float& value) const;

	Vector3 operator*(const float& value) const;
	Vector3 operator/(const float& value) const;

	// 벡터 개념 존재 x
	void operator+=(const float& value);
	void operator-=(const float& value);
	void operator*=(const float& value);
	void operator/=(const float& value);

	friend Vector3 operator+(const float value1, const Vector3& value2);
	friend Vector3 operator-(const float value1, const Vector3& value2);
	friend Vector3 operator*(const float value1, const Vector3& value2);
	friend Vector3 operator/(const float value1, const Vector3& value2);

	float Length() const;		// 벡터의 길이 반환

	Vector3 Normal() const;		// 정규화된 벡터를 반환
	void Normalize();			// 현재 벡터를 정규화 시킴

	Vector3 Cross(const Vector3& value) const;	// 현재 벡터와 외적
	float Dot(const Vector3& value) const;		// 현재 벡터와 내적

	static Vector3 Cross(const Vector3& vec1, const Vector3& vec2);	// 두 벡터를 외적
	static float Dot(const Vector3& vec1, const Vector3& vec2);	// 두 벡터를 내적

};