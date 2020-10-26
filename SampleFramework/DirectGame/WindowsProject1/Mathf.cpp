#include "Mathf.h"

float Mathf::Lerp(float a, float b, float t)
{
    return a + (b - a) * t;
}

float Mathf::Max(float a, float b)
{
    return a > b ? a : b;
}

float Mathf::Min(float a, float b)
{
    return a < b ? a : b;
}

int Mathf::Sign(float x)
{
    return x > 0 ? 1 : (x < 0 ? -1 : 0);
}

float Mathf::Clamp(float target, float inf, float sup)
{
    return Max(inf, Min(target, sup));
}

bool Mathf::InRange(float target, float inf, float sup)
{
    return inf <= target && target <= sup;
}

float Mathf::Abs(float x)
{
    return x > 0 ? x : -x;
}