#pragma once

namespace GameMath
{
	float Saturate(const float& value);

	int Random(int min, int max);
	float Random(float min, float max);

	float Distance(const Vector3& v1, const Vector3& v2);
}