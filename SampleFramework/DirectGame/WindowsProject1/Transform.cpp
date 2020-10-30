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

bool RectF::TouchOrIntersect(const RectF& rect1, const RectF& rect2)
{
    return  rect2.left <= rect1.right && rect2.right >= rect1.left &&
            rect2.top >= rect1.bottom && rect2.bottom <= rect1.top;
}

bool RectF::Contains(const RectF& other)
{
    return (
        this->left <= other.left &&
        this->top <= other.top &&
        this->right >= other.right &&
        this->bottom >= other.bottom);
}
