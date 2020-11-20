#include "Transform.h"

Vector2 VectorInfinity()
{
    return Vector2(999999.0f, 999999.0f);
}

Vector2 VectorZero()
{
    return Vector2(0.0f, 0.0f);
}

Vector2 NormalizedVector()
{
    return Vector2(1.0f, 1.0f);
}

Vector2 Multiply(float k, Vector2 vec)
{
    return Vector2(k * vec.x, k * vec.y);
}

Transform CreateTransform(Vector2 position, Vector2 scale, float rotation)
{
    Transform result;
    result.Position = position;
    result.Scale = scale;
    result.Rotation = rotation;
    return result;
}

bool RectF::TouchOrIntersect(const RectF& other)
{
    return  this->left <= other.right &&
        this->top <= other.bottom &&
        this->right >= other.left &&
        this->bottom >= other.top;
}

bool RectF::Contains(const RectF& other)
{
    return (
        this->left <= other.left &&
        this->top <= other.top &&
        this->right >= other.right &&
        this->bottom >= other.bottom);
}

bool RectF::Contains(const Vector2& point)
{
    return left <= point.x && 
            top <= point.y && 
            right >= point.x &&
            bottom >= point.y;
}
