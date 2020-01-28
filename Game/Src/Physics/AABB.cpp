#include "Precompiled.h"
#include "Physics/AABB.h"

#include <algorithm>

void AABB::SetDimensions(float x, float y)
{
	SetDimensions({ x, y });
}

void AABB::SetDimensions(Vec2 newDim)
{
	Vec2 halfInc = newDim * 0.5f;
	mTopRight = halfInc;
	mBottomLeft = -halfInc;
}

void AABB::Grow(Vec2 increase)
{
	Vec2 halfInc = increase * 0.5f;
	mTopRight += halfInc;
	mBottomLeft -= halfInc;
}

void AABB::Move(float x, float y)
{
	mPos += {x, y};
}

float AABB::GetLeftExtend() const
{
    return mPos.x + mBottomLeft.x;
}

float AABB::GetRightExtend() const
{
    return mPos.x + mTopRight.x;
}

float AABB::GetTopExtend() const
{
    return mPos.y + mTopRight.y;
}

float AABB::GetBottomExtend() const
{
	return mPos.y + mBottomLeft.y;
}

bool AABB::Collides(const AABB& other) const
{
	return (GetLeftExtend() < other.GetRightExtend() &&
			GetRightExtend() > other.GetLeftExtend() &&
			GetTopExtend() > other.GetBottomExtend() &&
			GetBottomExtend() < other.GetTopExtend());
}
