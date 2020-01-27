#include "Precompiled.h"
#include "Physics/AABB.h"

#include <algorithm>

void AABB::Grow(Vec2 increase)
{
    mWidth += increase.x;
    mHeight += increase.y;
    if (increase.x < 0.0f)
    {
        mX += increase.x;
    }
    if (increase.y < 0.0f)
    {
        mY += increase.y;
    }
}

int64_t AABB::GetLeftExtend() const
{
    return std::min(mX, mX + mWidth);
}

int64_t AABB::GetRightExtend() const
{
    return std::max(mX, mX + mWidth);
}

int64_t AABB::GetTopExtend() const
{
    return std::max(mY, mY + mHeight);
}

int64_t AABB::GetBottomExtend() const
{
    return std::min(mY, mY + mHeight);
}

bool AABB::Collides(const AABB& other) const
{
    return (GetLeftExtend() < other.GetRightExtend() &&
            GetRightExtend() > other.GetLeftExtend() &&
            GetTopExtend() > other.GetBottomExtend() &&
            GetBottomExtend() < other.GetTopExtend());
}
