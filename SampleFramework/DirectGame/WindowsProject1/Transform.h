#pragma once
#include <d3d9.h>
#include <d3dx9.h>

typedef D3DXVECTOR2 Vector2;
typedef D3DXVECTOR3 Vector3;

class Transform
{
public:
	Vector2 Position;
	Vector2 Scale;
	float Rotation;
};

struct RectF
{
	float left, top, right, bottom;
	bool TouchOrIntersect(const RectF& other);
	bool Contains(const RectF& other);
	bool Contains(const Vector2& point);
};

Vector2 VectorInfinity();
Vector2 VectorZero();
Vector2 NormalizedVector();
Vector2 Multiply(float k, Vector2 vec);
Transform CreateTransform(Vector2 position, Vector2 scale, float rotation);

