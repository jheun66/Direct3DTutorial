#include "Framework.h"

float GameMath::Saturate(const float& value)
{
    return max(0.0f, min(1.0f, value));
}

int GameMath::Random(int min, int max)
{
    return rand() % (max - min) + min;
}

float GameMath::Random(float min, float max)
{
    float normal = rand() / (float)RAND_MAX;

    return min + (max - min) * normal;
}

float GameMath::Distance(const Vector3& v1, const Vector3& v2)
{
    return (v2-v1).Length();
}

Vector3 GameMath::ClosestPointOnLineSegment(const Vector3& A, const Vector3& B, const Vector3& point)
{
    Vector3 AB = B - A;
    float t = AB.Dot(point - A) / AB.Dot(AB);

    t = Saturate(t);

    return A + t * AB;
}

Vector3 GameMath::WorldToScreen(const Vector3& pos)
{
    Vector3 screenPos;

    screenPos = XMVector3TransformCoord(pos.data, CAMERA->GetView());
    screenPos = XMVector3TransformCoord(screenPos.data, Environment::Get()->GetPerspective());

    screenPos.y *= -1;

    screenPos = (screenPos + 1.0f) * 0.5f;

    screenPos.x *= WIN_WIDTH;
    screenPos.y *= WIN_HEIGHT;

    return screenPos;
}
