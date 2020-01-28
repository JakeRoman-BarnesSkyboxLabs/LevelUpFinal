#pragma once
#include "Math/Vec2.h"

class AABB
{
public:
	Vec2 mBottomLeft;
	Vec2 mTopRight;
	Vec2 mPos;

public:
	AABB() = default;
	AABB(Vec2 pos) : mPos(pos) {}
	AABB(Vec2 pos, float width, float Height) : mPos(pos) { SetDimensions(width, Height); }

	void SetPosition(float x, float y) { mPos.x = x; mPos.y = y; }
	void SetPosition(Vec2 pos) { mPos = pos; }
	void SetDimensions(float x, float y);
	void SetDimensions(Vec2 newDim);
	void Grow(Vec2 increase);
	void Move(float x, float y);

	Vec2 GetCenter() const { return mPos; }
	float GetWidth() const { return mTopRight.x - mBottomLeft.x; }
	float GetHeight() const { return mTopRight.y - mBottomLeft.y; }
	float GetLeftExtend() const;
	float GetRightExtend() const;
	float GetTopExtend() const;
	float GetBottomExtend() const;
	bool Collides(const AABB& other) const;
};