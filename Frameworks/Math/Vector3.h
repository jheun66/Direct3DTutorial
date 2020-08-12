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

	// ∫§≈Õ ∞≥≥‰¿∫ ¡∏¿Á x
	Vector3 operator*(const Vector3& value) const;
	Vector3 operator/(const Vector3& value) const;

	void operator+=(const Vector3& value);
	void operator-=(const Vector3& value);
	void operator*=(const Vector3& value);
	void operator/=(const Vector3& value);

	// ∫§≈Õ ∞≥≥‰¿∫ ¡∏¿Á x
	Vector3 operator+(const float& value) const;
	Vector3 operator-(const float& value) const;

	Vector3 operator*(const float& value) const;
	Vector3 operator/(const float& value) const;

	friend Vector3 operator+(const float value1, const Vector3& value2);
	friend Vector3 operator-(const float value1, const Vector3& value2);
	friend Vector3 operator*(const float value1, const Vector3& value2);
	friend Vector3 operator/(const float value1, const Vector3& value2);
};