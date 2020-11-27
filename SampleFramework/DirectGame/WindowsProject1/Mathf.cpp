#include "Mathf.h"
#include <cstdlib>
#include <ctime>

const float Mathf::Pi = 3.1416f;

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

float Mathf::Sin(float x)
{
    return sin(x);
}

float Mathf::Cos(float x)
{
    return cos(x);
}

float Mathf::Tan(float x)
{
    return tan(x);
}

float Mathf::Arctan(float radian)
{
    return atan(radian);
}

float Mathf::Arctan2(float y, float x)
{
    return atan2(y, x);
}

float Mathf::Rad2Deg(float radian)
{
    return (radian / Pi) * 180.0f;
}

float Mathf::Deg2Rad(float degree)
{
    return degree * Pi / 180.0f;
}

float Mathf::Magnitude(Vector2 vec)
{
    return sqrt((vec.x * vec.x) + (vec.y * vec.y));
}

Vector2 Mathf::Normalize(Vector2 vec)
{
    auto magnitude = Magnitude(vec);
    return Vector2(vec.x / magnitude, vec.y / magnitude);
}

float Mathf::ToAngle(Vector2 vec)
{
    return Arctan2(vec.y, vec.x);
}

Vector2 Mathf::ToDirectionalVector(float radian)
{
    return Normalize(Vector2(Cos(radian), Sin(radian)));
}

bool Random::initialized = false;

// Random an integer in [inf, sup] (bounds included)
int Random::Range(int inf, int sup)
{
    if (initialized == false)
    {
        srand(time(NULL));
        initialized = true;
    }

    return (rand() % sup) + inf;
}
