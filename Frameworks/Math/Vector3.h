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

	// ���� ������ ���� x
	Vector3 operator*(const Vector3& value) const;
	Vector3 operator/(const Vector3& value) const;

	void operator+=(const Vector3& value);
	void operator-=(const Vector3& value);
	void operator*=(const Vector3& value);
	void operator/=(const Vector3& value);

	// ���� ������ ���� x
	Vector3 operator+(const float& value) const;
	Vector3 operator-(const float& value) const;

	Vector3 operator*(const float& value) const;
	Vector3 operator/(const float& value) const;

	// ���� ���� ���� x
	void operator+=(const float& value);
	void operator-=(const float& value);
	void operator*=(const float& value);
	void operator/=(const float& value);

	friend Vector3 operator+(const float value1, const Vector3& value2);
	friend Vector3 operator-(const float value1, const Vector3& value2);
	friend Vector3 operator*(const float value1, const Vector3& value2);
	friend Vector3 operator/(const float value1, const Vector3& value2);

	float Length() const;		// ������ ���� ��ȯ

	Vector3 Normal() const;		// ����ȭ�� ���͸� ��ȯ
	void Normalize();			// ���� ���͸� ����ȭ ��Ŵ

	Vector3 Cross(const Vector3& value) const;	// ���� ���Ϳ� ����
	float Dot(const Vector3& value) const;		// ���� ���Ϳ� ����

	static Vector3 Cross(const Vector3& vec1, const Vector3& vec2);	// �� ���͸� ����
	static float Dot(const Vector3& vec1, const Vector3& vec2);	// �� ���͸� ����

};