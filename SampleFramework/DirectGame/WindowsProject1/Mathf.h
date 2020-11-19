#pragma once
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
};

class Random
{
public:
	static int Range(int inf, int sup);

private:
	static bool initialized;
};

