#pragma once
#include "Transform.h"
class Mathf
{
public:
	static const float Pi;
	static float Lerp(float a, float b, float t);
	static float Max(float a, float b);
	static float Min(float a, float b);
	static int Sign(float x);
	static float Clamp(float target, float inf, float sup);
	static bool InRange(float target, float inf, float sup);
	static float Abs(float x);
	static float Sin(float x);
	static float Cos(float x);
	static float Tan(float x);
	static float Arctan(float radian);
	static float Arctan2(float y, float x);
	static float Rad2Deg(float radian);
	static float Deg2Rad(float degree);
	static float Magnitude(Vector2 vec);
	static Vector2 Normalize(Vector2 vec);
	static float ToAngle(Vector2 vec);
	static Vector2 ToDirectionalVector(float radian);
};

class Random
{
public:
	static int Range(int inf, int sup);

private:
	static bool initialized;
};

