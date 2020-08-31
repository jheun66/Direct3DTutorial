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
