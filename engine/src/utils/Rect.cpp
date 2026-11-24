#include "utils/Rect.h"

namespace engine
{
namespace
{
float right(const Rect& rect)
{
    return rect.position.x + rect.size.x;
}

float bottom(const Rect& rect)
{
    return rect.position.y + rect.size.y;
}
} // namespace

bool Rect::contains(const Vector2& point) const
{
    return point.x >= position.x && point.x < right(*this) && point.y >= position.y && point.y < bottom(*this);
}

bool Rect::contains(const Rect& other) const
{
    return other.position.x >= position.x && right(other) <= right(*this) && other.position.y >= position.y &&
           bottom(other) <= bottom(*this);
}

bool Rect::intersects(const Rect& other) const
{
    return position.x < right(other) && other.position.x < right(*this) && position.y < bottom(other) &&
           other.position.y < bottom(*this);
}
} // namespace engine
